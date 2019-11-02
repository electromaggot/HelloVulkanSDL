//
// DeviceAssessment.h
//	Vulkan Device Assist
//
// Encapsulate evaluation of Vulkan-supportive Devices, especially to
//	partially initialize DeviceProfiles and assist their later ranking.
//
// Pass-through to subordinate class DeviceRanking and RankDevices().
//	Provide convenient access to each DeviceProfile via index.
//
// Each device is initially queried for everthing that will possibly be asked of it, then
//	the list of devices ranked, before a single device is selected.  Subsequently, certain
//	stored values are referenced again for that device's setup, like loading extensions,
//	establishing the swapchain, etc.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DeviceAssay_h
#define DeviceAssay_h

#include "iPlatform.h"
#include "DeviceQueues.h"
#include "DeviceProfile.h"
#include "DeviceRanking.h"


class DeviceAssessment
{
public:
	DeviceProfile& operator [] (int index) {
		return deviceProfiles[index];
	}

	DeviceAssessment(VkPhysicalDevice devices[], int nDevices,
					 DeviceQueues& queues, VkSurfaceKHR& surface);
	~DeviceAssessment();

		// MEMBERS
private:
	DeviceProfile*	deviceProfiles;
	DeviceRanking*	deviceRanking;

	VkSurfaceKHR&	windowSurface;

		// METHODS
public:
	string RankDevices(VkPhysicalDevice devices[], int nDevices, int* rankedIndices);
private:
	void assayPhysicalDevices(VkPhysicalDevice devices[], int nDevices);
	bool assaySupportByDevice(VkPhysicalDevice device, int iDevice);

	void assayDeviceExtensionSupport(VkPhysicalDevice device, int iDevice);
	void assaySurfaceFormatSupport(VkPhysicalDevice device, int iDevice);
	void assayPresentModeSupport(VkPhysicalDevice device, int iDevice);
};

#endif // DeviceAssay_h
