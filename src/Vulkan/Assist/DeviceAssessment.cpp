//
// DeviceAssessment.cpp
//	Vulkan Device Assist
//
// See matched header file for definitive comment.
//
// Upon construction, query & store to Profiles:
//	- supported extensions
//	- arbitrary "score" for surface support:
//		- pixel depth/format & color space
//		- present modes
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "DeviceAssessment.h"
#include "Utility.h"
#include "PlatformSpecifics.h"	// (for __emplace_back on iOS, possibly)


DeviceAssessment::DeviceAssessment(VkPhysicalDevice devices[], int nDevices,
								   DeviceQueues& queues, VkSurfaceKHR& surface)
	:	deviceProfiles(new DeviceProfile[nDevices]),
		deviceRanking(new DeviceRanking((ProfilesReference) *deviceProfiles, nDevices, queues, surface)),
		windowSurface(surface)
{
	assayPhysicalDevices(devices, nDevices);
}

DeviceAssessment::~DeviceAssessment()
{
	delete	 deviceRanking;
	delete[] deviceProfiles;
}


string DeviceAssessment::RankDevices(VkPhysicalDevice devices[], int nDevices, int* rankedIndices)
{
	return deviceRanking->RankDevices(devices, nDevices, rankedIndices);
}


void DeviceAssessment::assayPhysicalDevices(VkPhysicalDevice devices[], int nDevices)
{
	int nSupportingDevices = 0;

	Log(NOTE, "GPUs found: %u", nDevices);

	for (int iDevice = 0; iDevice < nDevices; ++iDevice)
	{
		VkPhysicalDeviceProperties& deviceProperties = deviceProfiles[iDevice].properties;
		vkGetPhysicalDeviceProperties(devices[iDevice], &deviceProperties);
		Log(NOTE, "  %d: %s", iDevice, deviceProperties.deviceName);

		if (assaySupportByDevice(devices[iDevice], iDevice))
			++nSupportingDevices;
	}
	if (nSupportingDevices == 0)
		Fatal("No Device (of " + to_string(nDevices) + ") supports Required Device Extension: "
													+ stringAllREQUIRED_DEVICE_EXTENSION_NAMES());
}

bool DeviceAssessment::assaySupportByDevice(VkPhysicalDevice device, int iDevice)
{
	assayDeviceExtensionSupport(device, iDevice);

	assaySurfaceFormatSupport(device, iDevice);
	assayPresentModeSupport(device, iDevice);

	return !deviceProfiles[iDevice].lacksRequiredExtensions;
}


// Required Device Extensions:
//	- #1 factor in Device selection.
//	- Device not supporting REQUIRED_ Extension CANNOT be selected.
//	- If no supporting devices found, FATAL.  Check early.
//	- Multiple supporting devices are then subordinately ranked.
// Non-required Device Extensions:
//	- Device that Supports is more attractive than one that does not.
//	- Ranking level arbitrary.
//	- If not supported, continue but log and exclude from requested names.

void DeviceAssessment::assayDeviceExtensionSupport(VkPhysicalDevice device, int iDevice)
{
	uint32_t nDeviceExtensions;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &nDeviceExtensions, nullptr);
												//^^^^(layer name)
	VkExtensionProperties extensionProperties[nDeviceExtensions];
	vkEnumerateDeviceExtensionProperties(device, nullptr, &nDeviceExtensions, extensionProperties);

	for (int iExtension = 0; iExtension < N_DEVICE_EXTENSION_NAMES; ++iExtension) {
		auto requestExtensionName = DEVICE_EXTENSION_NAMES[iExtension];
		bool isExtensionUnsupportedByThisDevice = true;
		for (const auto& extensionProperty : extensionProperties) {
			auto supportedExtensionName = extensionProperty.extensionName;
			if (strcmp(requestExtensionName, supportedExtensionName) == 0) {
				isExtensionUnsupportedByThisDevice = false;
				break;
			}
		}
		if (isExtensionUnsupportedByThisDevice) {
			if (REQUIRE_DEVICE_EXTENSION[iExtension]) {
				deviceProfiles[iDevice].lacksRequiredExtensions = true;
				return;		// device cannot be selectable
			}
			else {	// optional extension:
				Log(WARN, "Desired Device Extension non-supported: " + string(requestExtensionName));
				continue;	// this is okay, but won't contribute to extension count, hence rank
			}
		}
		deviceProfiles[iDevice].extensionNames.emplace_back(requestExtensionName);
	}
}

// For the given Device and stored windowSurface, compare their surfaceFormat.format to our
//	PIXEL_FORMAT_PRECEDENCE and their surfaceFormat.colorSpace to our COLOR_SPACE_PRECEDENCE,
//	generating a higher score for most-favored format/colorSpace.  Store "chosen" ones.
// NOTE: SurfaceFormat .format and .colorSpace come PAIRED.  Keep them that way.
//
void DeviceAssessment::assaySurfaceFormatSupport(VkPhysicalDevice device, int iDevice)
{
	uint32_t nFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface, &nFormats, nullptr);
	if (nFormats == 0) {
		Log(ERROR, "No supported Surface Formats reported by Device " + to_string(iDevice));
		return;
	}
	VkSurfaceFormatKHR formats[nFormats];
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface, &nFormats, formats);

	deviceProfiles[iDevice].surfaceSupportScore = 0;
	bool yetToMatchFormat = true, yetToMatchColorSpace = true;
	const char* choseNonpreferred = nullptr;

	for (auto& currentFormat : formats)
	{
		RawScore formatScore = 0, colorSpaceScore = 0;

		for (int iPrecedence = 0; iPrecedence < N_PIXEL_FORMAT_PRECEDENCES; ++iPrecedence)
			if (currentFormat.format == PIXEL_FORMAT_PRECEDENCE[iPrecedence]) {
				formatScore = N_PRECEDENCES - iPrecedence;
				yetToMatchFormat = false;
				break;
			}
		for (int iPrecedence = 0; iPrecedence < N_COLOR_SPACE_PRECEDENCES; ++iPrecedence)
			if (currentFormat.colorSpace == COLOR_SPACE_PRECEDENCE[iPrecedence]) {
				colorSpaceScore = N_PRECEDENCES - iPrecedence;
				yetToMatchColorSpace = false;
				break;
			}
		RawScore combinedScore = formatScore + colorSpaceScore;

		if (combinedScore > deviceProfiles[iDevice].surfaceSupportScore)
		{
			deviceProfiles[iDevice].surfaceSupportScore = combinedScore;
			deviceProfiles[iDevice].selectedSurfaceFormat = currentFormat;

			choseNonpreferred = formatScore == 0 ? "PIXEL FORMAT" :
								colorSpaceScore == 0 ? "COLOR SPACE" : nullptr;
		}
	}
	if (deviceProfiles[iDevice].surfaceSupportScore == 0) {	// no choice made
		for (auto& surfaceFormat : formats)		// see if we can at least find one that's not UNDEFINED
			if (surfaceFormat.format != VK_FORMAT_UNDEFINED) {
				Log(WARN, "Failed to find better SurfaceFormat, choosing first one that's not VK_FORMAT_UNDEFINED.");
				deviceProfiles[iDevice].selectedSurfaceFormat = surfaceFormat;
				goto skipForElse;
			}
		Log(WARN, "Physical Device Surface Format UNDEFINED; last-ditch defaulting to 32-bit UNORM SRGB.");
		deviceProfiles[iDevice].selectedSurfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		skipForElse:;
	} else if (choseNonpreferred)
		Log(WARN, "Chosen SurfaceFormat non-preferred, unable to pair matched " + string(choseNonpreferred) + ".");
	if (yetToMatchFormat)
		Log(WARN, "Device " + to_string(iDevice) + " supports NONE of " + to_string(N_PIXEL_FORMAT_PRECEDENCES) + " requested/preferred Pixel Formats.");
	if (yetToMatchColorSpace)
		Log(WARN, "Device " + to_string(iDevice) + " supports NONE of " + to_string(N_COLOR_SPACE_PRECEDENCES) + " requested/preferred Color Spaces.");
}
// Scoring for the above & below, explained:
//	- each _PRECEDENCE array:
//		- carries equal weight(*), highest-priority score from each should be the same.
//		- items listed first are higher-priority == higher-scoring; subsequent items descend.
//	- INDEX into _PRECEDENCE array:
//		- ascends starting at 0, so score must be the "reverse."  Easily done by:
//		- SCORE = LARGE_VALUE - INDEX
//	- what LARGE_VALUE to use?
//		- avoid arbitrary value (say, 100) i.e. no "just hope it's never exceeded" limit.
//		- can't use any single _PRECEDENCE array size: violates (*) above.
//		- solution: will use all _PRECEDENCES array sizes summed together.
//		- this has an added benefit: two summed low-priorty matches can still -and should-
//		  outweigh one higher-priority match summed with a *no match* zero.
//	- individual swapchain-related scores add to one accumulated score.
//	- if NO MATCH: log error, contribute 0 to overall score: discourage device's selection.


// For the given Device and stored windowSurface, compare their PresentModes to our
//	PRESENT_MODE_PRECEDENCE, generating a higher score for more favored modes.
//	Also, store the Mode that will eventually be used as the one most-favored.
//
void DeviceAssessment::assayPresentModeSupport(VkPhysicalDevice device, int iDevice)
{
	uint32_t nModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface, &nModes, nullptr);
	if (nModes == 0) {
		Log(WARN, "No supported Surface Present Modes reported by Device " + to_string(iDevice));
		return;
	}
	VkPresentModeKHR presentModes[nModes];
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface, &nModes, presentModes);

	for (int iPrecedence = 0; iPrecedence < N_PRESENT_MODE_PRECEDENCES; ++iPrecedence)
		for (auto& deviceMode : presentModes)
			if (deviceMode == PRESENT_MODE_PRECEDENCE[iPrecedence])
			{
				RawScore score = N_PRECEDENCES - iPrecedence;
				deviceProfiles[iDevice].surfaceSupportScore += score;
				deviceProfiles[iDevice].selectedPresentMode = deviceMode;
				return;
			}
	Log(WARN, "Device " + to_string(iDevice) + " supports NONE of " + to_string(N_PRESENT_MODE_PRECEDENCES) + " requested/preferred Present Modes.");

	Log(WARN, "Sans a better PresentMode, choosing first one reported: " + to_string(presentModes[0]));
	deviceProfiles[iDevice].selectedPresentMode = presentModes[0];
}
