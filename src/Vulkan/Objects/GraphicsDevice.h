//
// GraphicsDevice.h
//	Vulkan Setup
//
// Abstract the graphics card/GPU.
//
// Nomenclature note - meaning of
//	prefixes to variable names:
//		n = "number of" items in collection, or size/count
//		i = "index of" item in array, vector, or similar collection
//		is = boolean prefix, also: did  ...perhaps other verb
//	additional conventions:
//		rr or call = "return result" from a function call
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DeviceAbstract_h
#define DeviceAbstract_h

#include "VulkanInstance.h"
#include "ValidationLayers.h"
#include "WindowSurface.h"
#include "DeviceQueues.h"
#include "DeviceAssessment.h"


class GraphicsDevice
{
private:
	GraphicsDevice();
public:
	GraphicsDevice(WindowSurface& surface, VulkanInstance& vulkan, ValidationLayers& validation);
	~GraphicsDevice();

	DeviceQueues&		Queues;				// each device's queues/families

		// MEMBERS
private:
	VkDevice			logicalDevice;

	VkPhysicalDevice	physicalDevice;		// which GPU is SELECTED
	DeviceProfile		selected;
	string				reason;				// and why!

	DeviceQueues		queueFamilies;

		// METHODS
private:
	void createLogicalDevice(ValidationLayers& validation);
	void determineDeviceExtensionSupport(VkPhysicalDevice* devices, int nDevices, DeviceAssessment& assays);

	VkPhysicalDevice selectGPU(VkInstance& instance, VkSurfaceKHR& surface,
							   DeviceSelectionMethod choice = DEVICE_SELECTION_MODE);
	VkPhysicalDevice manuallySelectGPU(VkInstance& instance, VkSurfaceKHR& surface);
	int consolePromptUserToChooseDevice(int nDevices);

	int pickBestDevice(VkPhysicalDevice devices[], int nDevices, DeviceAssessment& deviceAssess);
	int pickSpecifiedDevice(DeviceAssessment& deviceAssess, int nDevices);

public:
	bool IsImageFormatSupported(VkFormat format, VkImageTiling tiling = (VkImageTiling) -1);

		// getters
	VkPhysicalDevice& getGPU()	{ return physicalDevice; }
	VkDevice& getLogical()		{ return logicalDevice;  }
	DeviceProfile& getProfile()	{ return selected;		 }
};

#endif // DeviceAbstract_h
