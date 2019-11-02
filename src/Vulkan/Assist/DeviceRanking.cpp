//
// DeviceRanking.cpp
//	Vulkan Device Assist
//
// See matched header file for definitive comment.
//
// Implementation goals herein -
// Scoring scheme favors:		i.e. defaulting to:
//	- dedicated GPU (vs. integrated)
//	- support for graphics and present operations
//	- availability of desired device extensions
//	- larger VRAM
//	- higher-end or newer model (hard to determine?)
//	(other options: higher-performance/faster, greater extension support)
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "DeviceRanking.h"
#include "Utility.h"


DeviceRanking	*DeviceRanking::self;

typedef uint64_t	Score;


DeviceRanking::DeviceRanking(ProfilesReference profiles, int nDevices,
							 DeviceQueues& queues, VkSurfaceKHR& surface)
	:	deviceQueues(queues),
		windowSurface(surface),
		deviceProfiles(profiles)
{
	self = this;
}

DeviceRanking::~DeviceRanking()
{
	self = nullptr;
}


// Return-writes an array of indices of devices ranked in order, best to worst (0 ..< nDevices).
//	Also will log (pending "Note" level) each score vs. assessment of the device's capabilities.
// Scores are tallied in successive passes, according to an array --in order of importance-- of
//	functions that return "raw" scores.  Those can be compared against each other, for the current
//	pass, to form a "normalized" score suitable for comparison across passes... which is then
//	added into each device's total.  Presently, the total is multiplied each pass (to maintain
//	the aforementioned order-of-importance) by the maximum normalized value.  This approach keeps
//	the significance of each pass distinct, but a smaller multiplier could be used to "blend" it.
//	The rationale to this scoring algorithm is further discussed in the Developer Notes.
//
string DeviceRanking::RankDevices(VkPhysicalDevice devices[], int nDevices, int rankedIndices[])
{
	RawScore (*passOrder[])(VkPhysicalDevice&, DeviceProfile&) = {

		&quantifyDeviceType,		// Favor Discrete GPU foremost;
		&gaugeQueueFamilySupport,	// then best support for graphics/present operations;
		&countDesiredExtensions,	// then presence of non-required Device Extensions;
		&factorInSurfaceSupport,	// then favored color depth/space & present mode;
		&getNVRAMsize,				// then largest VRAM;
		&quantifyAPIversion,		// then newer Vulkan API;
		&getMaxImageDimension2D		// and finally, biggest supported image size.
	};
	// (unfortunately, still can't figure out how to get GPU frequency/performance)

	const char* delimit = "";
	for (int iPass = 0; iPass < N_ELEMENTS_IN_ARRAY(passOrder); ++iPass, delimit = ", ")
	{
		uint64_t greatestScore = 0;
		for (int iDevice = 0; iDevice < nDevices; ++iDevice)
		{
			if (deviceProfiles[iDevice].lacksRequiredExtensions) {
				deviceProfiles[iDevice].description = "Non-selectable, lacks REQUIRED Device Extension";
				continue;
			}

			deviceProfiles[iDevice].description += delimit;

			uint64_t eachScore = passOrder[iPass](devices[iDevice], deviceProfiles[iDevice]);
			deviceProfiles[iDevice].rawPerPassScore = eachScore;

			if (eachScore > greatestScore)
				greatestScore = eachScore;
		}
		const Score NORMALIZE_MAX = nDevices;	// Score's range will be normalized to 0..NORMALIZED_MAX range.
		// Set this ^ at 10 to view a "decimalized" version of the score, which helps with debugging! (however in that case, it arbitrarily couples to 10 devices)
		for (int i = 0; i < nDevices; ++i) {
			DeviceProfile& assay = deviceProfiles[i];

			Score normalizedScore = 0;
			if (greatestScore > 0)
				normalizedScore = (Score) (assay.rawPerPassScore * (NORMALIZE_MAX - 1) / greatestScore);
			assay.totalScore *= NORMALIZE_MAX;	// Scale up previous pass score, which was more important.
			assay.totalScore += normalizedScore;
		}
	}

	Log(NOTE, "Device: Score = reason");
	for (int iDevice = 0; iDevice < nDevices; ++iDevice)
		Log(NOTE, "%6d: %3u = %s", iDevice, deviceProfiles[iDevice].totalScore, deviceProfiles[iDevice].description.data());

	for (int iRank = 0; iRank < nDevices; ++iRank) {	// Beware: this loop destroys each .totalScore
		Score highestScore = 0;							//	to write output-array of device Indexes.
		int highestIndex = -1;
		for (int iDevice = 0; iDevice < nDevices; ++iDevice) {
			Score deviceScore = deviceProfiles[iDevice].totalScore;
			if (deviceScore > highestScore) {
				highestScore = deviceScore;
				highestIndex = iDevice;
			}
		}
		rankedIndices[iRank] = highestIndex;			// Note that ties in score are still "ranked" per device order.
		deviceProfiles[highestIndex].totalScore = 0;
	}
	string reason = deviceProfiles[rankedIndices[0]].description;

	string note = "Ranked device indices, in order: ";
	for (int iIndex = 0; iIndex < nDevices; ++iIndex)
		note += (iIndex > 0 ? ", " : "") + to_string(rankedIndices[iIndex]);
	Log(NOTE, note);

	return reason;
}

RawScore DeviceRanking::quantifyDeviceType(VkPhysicalDevice& device, DeviceProfile& assay) {
	const uint64_t MAX_SCORE = 5;
	static struct deviceTypeScores {		// Constructing this table compile-time makes for efficient run-time lookup.
		deviceTypeScores() {
			score[VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU]	  = MAX_SCORE;		name[VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU]	 = "DISCRETE GPU  ";
			score[VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU] = MAX_SCORE - 1;	name[VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU] = "INTEGRATED GPU";
			score[VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU]	  = MAX_SCORE - 2;	name[VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU]	 = "VIRTUAL GPU   ";
			score[VK_PHYSICAL_DEVICE_TYPE_CPU]			  = MAX_SCORE - 3;	name[VK_PHYSICAL_DEVICE_TYPE_CPU]			 = "CPU           ";
			score[VK_PHYSICAL_DEVICE_TYPE_OTHER]		  = MAX_SCORE - 4;	name[VK_PHYSICAL_DEVICE_TYPE_OTHER]			 = "OTHER device  ";
		}
		uint64_t score[VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE];
		const char* name[VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE];
	} deviceTable;

	VkPhysicalDeviceType deviceType = assay.properties.deviceType;
	if (deviceType < 0 || deviceType >= VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE)
	{
		assay.description += "ERRONEOUS deviceType";
		return 0;
	}
	uint64_t score = deviceTable.score[deviceType];
	assay.description += deviceTable.name[deviceType] + string(" (rank #") + to_string(MAX_SCORE - score + 1) + ")";
	return score;
}

RawScore DeviceRanking::gaugeQueueFamilySupport(VkPhysicalDevice& device, DeviceProfile& assay) {
	DeviceQueues& queues = self->deviceQueues;
	queues.DetermineFamilyIndices(device, self->windowSurface);
	assay.description += "graphics/present: " + string(queues.suitabilityString());
	return queues.RankDeviceSuitability();
}

// Score simply by the number of Device Extensions supported by given Device.  This includes
//	both non-required "desired" Extensions, which is the count we care about for scoring, and
//	also incidentally includes REQUIRED_ Extensions <-- which BTW are not truly relevant here,
//	since all required extensions will always be required and if even one is unsupported, its
//	device it will be unavailable for selection (so its score wouldn't matter anyway).  The
//	Required count contributes the same for all selectable devices, not affecting ranking.
//
RawScore DeviceRanking::countDesiredExtensions(VkPhysicalDevice& device, DeviceProfile& assay) {

	size_t count = assay.extensionNames.size();
	assay.description += "extensions " + to_string(count);
	return count;
}

RawScore DeviceRanking::factorInSurfaceSupport(VkPhysicalDevice& device, DeviceProfile& assay) {
	RawScore score = assay.surfaceSupportScore;
	assay.description += "surfaceSupport " + to_string(score);
	return score;
}

RawScore DeviceRanking::getNVRAMsize(VkPhysicalDevice& device, DeviceProfile& assay) {
	uint64_t vramBytes = Utility::GetByteSizeVRAM(device);
	assay.description += "VRAM " + Utility::StringBytesWithUnitPrefix(vramBytes) + "bytes";
	return vramBytes;
}

RawScore DeviceRanking::quantifyAPIversion(VkPhysicalDevice& device, DeviceProfile& assay) {
	uint32_t versionValue = assay.properties.apiVersion;
	assay.description += "API version " + to_string(versionValue);
	return versionValue;
}

RawScore DeviceRanking::getMaxImageDimension2D(VkPhysicalDevice& device, DeviceProfile& assay) {
	uint32_t dimension = assay.properties.limits.maxImageDimension2D;
	assay.description += "maxImageDimension2D " + to_string(dimension);
	return dimension;
}
