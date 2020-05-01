//
// DeviceQueues.cpp
//	Vulkan Setup Module
//
// See header file for main comment.
//
// 2/2/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "DeviceQueues.h"


DeviceQueues::DeviceQueues()
	:	indexGraphicsFamily(INDEX_UNDEFINED),
		indexPresentFamily(INDEX_UNDEFINED),
		suitability(UNKNOWN)
{ }

// For the given physical device and display surface pair, query all Queue Families
//	for the specific ones that support Graphics commands and Present operation.
// Return an assessment of how "suitable" the queue/family is for our most typical
//	purpose, which is rendering graphics and presenting them to a display device.
//
QueueFitness DeviceQueues::DetermineFamilyIndices(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
{
	suitability = NOT_SUPPORTED;

	uint32_t nFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nFamilies, nullptr);
	if (nFamilies == 0)
		Log(ERROR, "Physical Device supports NO Queue Families.");
	else {
		VkQueueFamilyProperties familyProperties[nFamilies];
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nFamilies, familyProperties);

		for (int iFamily = 0; iFamily < nFamilies; ++iFamily)
		{
			VkQueueFamilyProperties& family = familyProperties[iFamily];

			if (family.queueCount > 0)
			{
				bool supportsGraphics = (family.queueFlags & VK_QUEUE_GRAPHICS_BIT);

				VkBool32 supportsPresent = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, iFamily, surface, &supportsPresent);

				if (supportsGraphics && supportsPresent) {	// Ideally one queue family supporting both
					indexGraphicsFamily = iFamily;			//	Graphics and Present is preferred,
					indexPresentFamily = iFamily;
					suitability = IDEAL_EXCLUSIVE;
					break;									//	so if found, quit early successfully.
				}
				if (supportsGraphics && indexGraphicsFamily == INDEX_UNDEFINED) { // Otherwise, accept non-overlapping
					indexGraphicsFamily = iFamily;								  //  families (favoring first-found)...
					suitability |= SUPPORTS_GRAPHICS;
				}
				if (supportsPresent && indexPresentFamily == INDEX_UNDEFINED) {
					indexPresentFamily = iFamily;
					suitability |= SUPPORTS_PRESENT;
				}																  //  ...and keep looking.
			}
		}
	}
	return suitability;
}

void DeviceQueues::logSupportAnomaly()
{
	if (suitability == UNKNOWN)
		Log(ERROR, "Call to AnalyzeFamilyIndices() never made.");
	else if (suitability < SUPPORTS_BOTH)
	{
		char allNames[] = "Graphics, Present";
		char* setNames = allNames;
		if (indexGraphicsFamily != INDEX_UNDEFINED) setNames = &allNames[10];
		if (indexPresentFamily != INDEX_UNDEFINED) allNames[8] = '\0';
		Log(ERROR, "Command Queue/Family not supported: %s", setNames);
	}

	if (QueueCreateInfos[0].sType != VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO)
		Log(ERROR, "QueueCreateInfos[] look uninitialized, call InitializeQueueCreateInfos() first.");
}

void DeviceQueues::InitializeQueueCreateInfos()
{
	float queuePriority = 1.0f;

	for (int iQueue = 0; iQueue < nIndices; ++iQueue)
	{
		QueueCreateInfos[iQueue] = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = familyIndices[iQueue],
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};
	}
	//TJ_TODO: must better handle indentical queues
}

void DeviceQueues::GatherQueueHandlesFor(VkDevice& logicalDevice)
{
	vkGetDeviceQueue(logicalDevice, indexGraphicsFamily, 0, &queueGraphics);
	vkGetDeviceQueue(logicalDevice, indexPresentFamily,  0, &queuePresent);
}
