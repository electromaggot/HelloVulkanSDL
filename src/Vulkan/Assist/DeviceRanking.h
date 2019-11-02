//
// DeviceRanking.h
//	Vulkan Device Assist
//
// Encapsulate evaluation, scoring, ranking of Vulkan-supportive Devices.
//
// Uses numerous 'static's, especially to mimic a "jump table,"
//	so only intended for single instantiation/singleton.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DeviceRanking_h
#define DeviceRanking_h

#include "iPlatform.h"
#include "DeviceQueues.h"
#include "DeviceProfile.h"


class DeviceRanking
{
public:
	DeviceRanking(ProfilesReference profiles, int nDevices,
				  DeviceQueues& queues, VkSurfaceKHR& surface);
	~DeviceRanking();
private:
	static DeviceRanking	*self;

	// MEMBERS
	DeviceQueues&	  deviceQueues;
	VkSurfaceKHR&	  windowSurface;

	ProfilesReference deviceProfiles;

	// METHODS
public:
	string RankDevices(VkPhysicalDevice devices[], int nDevices, int* rankedIndices);
private:
	static RawScore quantifyDeviceType(VkPhysicalDevice&, DeviceProfile&);
	static RawScore gaugeQueueFamilySupport(VkPhysicalDevice&, DeviceProfile&);
	static RawScore countDesiredExtensions(VkPhysicalDevice&, DeviceProfile&);
	static RawScore factorInSurfaceSupport(VkPhysicalDevice& device, DeviceProfile& assay);
	static RawScore getNVRAMsize(VkPhysicalDevice&, DeviceProfile&);
	static RawScore quantifyAPIversion(VkPhysicalDevice&, DeviceProfile&);
	static RawScore getMaxImageDimension2D(VkPhysicalDevice&, DeviceProfile&);
};

#endif // DeviceRanking_h
