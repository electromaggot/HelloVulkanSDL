//
// GraphicsDevice.cpp
//	Vulkan Setup
//
// See matched header file for definitive high-level comment.
//
// Implementation goals herein:
//	- automatic device selection (when multiple GPUs are present)
//	  per ranking algorithm, see:  DeviceAssessment.cpp
//	- still retain a way to manually select GPU
//	- per desired queue family availability
//	- driver support of Vulkan capabilities
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "GraphicsDevice.h"
#include "VulkanConfigure.h"


GraphicsDevice::GraphicsDevice(WindowSurface& surface, VulkanInstance& instance,
													ValidationLayers& validation)
	:	Queues(queueFamilies),	// mimic a "readonly" reference
		physicalDevice(VK_NULL_HANDLE)
{
	VkSurfaceKHR vkSurface = surface.getVkSurface();

	physicalDevice = selectGPU(instance.getVkInstance(), vkSurface);

	// Now-selected physicalDevice may not have correct Indices set, so make sure:
	queueFamilies.DetermineFamilyIndices(physicalDevice, vkSurface);

	createLogicalDevice(validation);
}

GraphicsDevice::~GraphicsDevice()
{
	vkDestroyDevice(logicalDevice, nullALLOC);
}


bool GraphicsDevice::IsImageFormatSupported(VkFormat format, VkImageTiling tiling)
{
	VkFormatProperties	formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

	VkFormatFeatureFlags flags;

	switch (tiling) {
	case VK_IMAGE_TILING_OPTIMAL:
		flags = formatProperties.optimalTilingFeatures;
		break;
	case VK_IMAGE_TILING_LINEAR:
		flags = formatProperties.linearTilingFeatures;
		break;
	default:
		flags = formatProperties.bufferFeatures;
	}
	return (flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) != 0;
}


// Call vkCreateDevice, relying on an already-selected physicalDevice reference plus an array
//	of Extension names that it supports (built for that device at the time it was selected).
//
void GraphicsDevice::createLogicalDevice(ValidationLayers& validation)
{
	queueFamilies.InitializeQueueCreateInfos();

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {
		.sType	= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0,

		.queueCreateInfoCount = N_ELEMENTS_IN_ARRAY(queueFamilies.QueueCreateInfos),
		.pQueueCreateInfos	  = queueFamilies.QueueCreateInfos,

		.enabledLayerCount	 = validation.NumEnabledLayers(),
		.ppEnabledLayerNames = validation.ppEnabledLayerNames(),

		.enabledExtensionCount	 = (uint32_t) selected.extensionNames.size(),
		.ppEnabledExtensionNames = selected.extensionNames.data(),

		.pEnabledFeatures = &deviceFeatures
	};

	call = vkCreateDevice(physicalDevice, &createInfo, nullALLOC, &logicalDevice);

	if (call != VK_SUCCESS)
		Fatal("Create Logical Device FAILURE" + ErrStr(call));

	queueFamilies.GatherQueueHandlesFor(logicalDevice);
}


// SELECT PHYSICAL DEVICE, usually automatically.  (or manually otherwise rarely)
//
VkPhysicalDevice GraphicsDevice::selectGPU(VkInstance& instance, VkSurfaceKHR& surface,
										   DeviceSelectionMethod choice) {
	uint32_t nPhysicalDevices = 0;
	vkEnumeratePhysicalDevices(instance, &nPhysicalDevices, nullptr);
	if (nPhysicalDevices == 0)
		Fatal("No Physical Devices found with Vulkan support.");

	VkPhysicalDevice physicalDevices[nPhysicalDevices];
	vkEnumeratePhysicalDevices(instance, &nPhysicalDevices, physicalDevices);

	DeviceAssessment assessDevices(physicalDevices, nPhysicalDevices, Queues, surface);

	int iSelected = -1;
	StrPtr how = "Automatically";

	if (nPhysicalDevices == 1)
		iSelected = 0;
	else {
		if (choice == AUTOMATIC) {

			iSelected = pickBestDevice(physicalDevices, nPhysicalDevices, assessDevices);
		}
		else if (choice == SPECIFIED) {

			iSelected = pickSpecifiedDevice(assessDevices, nPhysicalDevices);
			how = "SPECIFIED device";
			reason = "\"" + string(DEVICE_NAME_IF_SPECIFIED) + "\" hard-coded";
		}
		if (iSelected < 0) {	// PROMPT_USER or if unable to automatically choose

			iSelected = consolePromptUserToChooseDevice(nPhysicalDevices);
			how = "Manually";
			reason = "user prompt on console";
		}
	}
	Log(NOTE, "%s selected: %s (reason: %s)", how,
			  assessDevices[iSelected].properties.deviceName, reason.data());

	selected = assessDevices[iSelected];	// persist copy (DeviceAssessment.deviceProfiles leaving scope)

	return physicalDevices[iSelected];
}

int GraphicsDevice::pickBestDevice(VkPhysicalDevice devices[], int nDevices,
								   DeviceAssessment& deviceAssess)
{
	assert(nDevices > 0 && devices != nullptr); // contract

	int ranks[nDevices];
	reason = deviceAssess.RankDevices(devices, nDevices, ranks);

	return ranks[0];
}

int GraphicsDevice::pickSpecifiedDevice(DeviceAssessment& assess, int nDevices)
{
	assert(nDevices > 0); // contract

	for (int iDevice = 0; iDevice < nDevices; ++iDevice)
		if (strcmp(assess[iDevice].properties.deviceName, DEVICE_NAME_IF_SPECIFIED) == 0)
			return iDevice;
	Log(ERROR, "DEVICE_NAME_IF_SPECIFIED \"" + string(DEVICE_NAME_IF_SPECIFIED)
			   + "\" unmatched amongst " + to_string(nDevices) + " available devices.");
	return -1;
}

// Prompt user to select physical graphics device.  (this isn't used much & not very important, available for convenience)
//
VkPhysicalDevice GraphicsDevice::manuallySelectGPU(VkInstance& instance, VkSurfaceKHR& surface)
{
	return selectGPU(instance, surface, PROMPT_USER);
}

int GraphicsDevice::consolePromptUserToChooseDevice(int nPhysicalDevices)
{
	int iSelected = -1;
	while (true)
	{
		std::cout << "Please select device, numeric index: ";
		std::cin  >> iSelected;
		if (iSelected >= 0 && iSelected < nPhysicalDevices)
			return iSelected;
		std::cout << "Invalid selection, enter a value from 0 to " << nPhysicalDevices - 1 << ".\n";
	}
}
