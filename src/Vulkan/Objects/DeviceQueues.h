//
// DeviceQueues.h
//	Vulkan Setup Module
//
// Encapsulate the Queue Families and Queues supported
//	by a Device for Graphics-related operations.
// Intended to be parented/owned/instanced by GraphicsDevice.
//
// The "suitability" analysis reflects how well a given device supports
//	both Graphics and "Present" operations in its queues (queue family)
//	particularly for the main purpose here: rendering and presenting
//	graphics on a display device.  Thus, ranked highest-to-lowest:
//	 1. prefer graphics and present concurrently on same queue (improved performance?)
//	 2. secondarily favor both graphics and present albeit via separate families
//	 3. graphics-only device, perhaps dedicated to off-screen rendering/off-server display
//	 4. present-only support doesn't make a lot of sense, but is still better than nothing
//	 5. if neither graphics nor present support, this device may not be suitable at all
//
// 2/2/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DeviceQueues_h
#define DeviceQueues_h

#include "VulkanPlatform.h"


#define INDEX_UNDEFINED		-1

enum QueueFitness {
	UNKNOWN			  = -1,
	NOT_SUPPORTED	  = 0b0000,
	SUPPORTS_PRESENT  = 0b0001,
	SUPPORTS_GRAPHICS = 0b0010,
	SUPPORTS_BOTH	  = 0b0011,
	IDEAL_EXCLUSIVE	  = 0b0100
};
inline QueueFitness operator |= (QueueFitness left, QueueFitness right) {
	return static_cast<QueueFitness>(static_cast<int>(left) | static_cast<int>(right));
}
static const char* stringFitness[] = {	"unsupported",
										"no graphics",
										"no present",
										"separate",
										"exclusive"  };

class DeviceQueues {
public:
	DeviceQueues();

		// MEMBERS
private:
	static const int nIndices = 2;			// Value must match, of course...
	union {
		struct {
			int32_t indexGraphicsFamily;	// 1
			int32_t indexPresentFamily;		// 2  ...quantity of these.
		};
		uint32_t familyIndices[nIndices];
	};

	VkQueue		 queueGraphics;
	VkQueue		 queuePresent;

	QueueFitness suitability;

public:
	VkDeviceQueueCreateInfo QueueCreateInfos[nIndices];	// (public so 'get' of sized-array is possible)

		// METHODS
public:
	QueueFitness DetermineFamilyIndices(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
	void GatherQueueHandlesFor(VkDevice& logicalDevice);
	void InitializeQueueCreateInfos();

	bool IsExclusiveGraphicsAndPresent() {
		bool isExclusive = (suitability == IDEAL_EXCLUSIVE);
		if ((isExclusive && indexGraphicsFamily != indexPresentFamily)
		 || (indexGraphicsFamily == INDEX_UNDEFINED && indexPresentFamily == INDEX_UNDEFINED))
			Log(WARN, "DeviceQueue EXCLUSIVITY queried but erroneous or UNDEFINED.");
		return isExclusive;
	}
private:
	void logSupportAnomaly();

		// getters
public:
	VkQueue& Graphics()				{ return queueGraphics;	}
	VkQueue& Present()				{ return queuePresent;	}

	typedef uint32_t (&IndexArrayRef)[nIndices];

	IndexArrayRef Indices()		{ return familyIndices; }

	uint32_t GraphicsIndex() {
		if (indexGraphicsFamily <= INDEX_UNDEFINED)
			Log(WARN, "Graphics Family Index accessed but UNDEFINED.");
		return indexGraphicsFamily;
	}
	uint32_t PresentIndex() {
		if (indexPresentFamily <= INDEX_UNDEFINED)
			Log(WARN, "Present Family Index accessed but UNDEFINED.");
		return indexPresentFamily;
	}

	// It happens that QueueFitness's enumeration values provide an appropriate
	//	"ranking" of device/queue suitability, where higher number is better.
	//
	int RankDeviceSuitability() {
		if (suitability == UNKNOWN) {
			Log(WARN, "Analyze device/queue suitability BEFORE requesting it.");
			return 0;
		}
		return (int) suitability;
	}

	const char* suitabilityString()	{ return stringFitness[suitability]; }
};

#endif // DeviceQueues_h
