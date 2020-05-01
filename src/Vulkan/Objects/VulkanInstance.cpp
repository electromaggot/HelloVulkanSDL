//
// VulkanInstance.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanInstance.h"
#include "VulkanSingleton.h"

#include "AppConstants.h"	// (one of only two tie-ins from our Vulkan module back to the App)

#undef __cpp_impl_three_way_comparison		//TJ: otherwise "spaceship operator" errors-out with: std::partial_ordering not supported
#include "vulkan/vulkan.hpp"		// for error to_string(VkResult)


const auto	TARGET_VULKAN_API_VERSION = VK_API_VERSION_1_0;

VkResult	call;			// return result, for all to use


VulkanInstance::VulkanInstance(ValidationLayers& layers, iPlatform& platform)
{
	createInstance(platform, layers);

	isDebugReportProvided = layers.IsValidationProvided() && platform.IsDebugReportProvided();
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(instance, nullALLOC);
}


void VulkanInstance::createInstance(iPlatform& platform, ValidationLayers& layers)
{
	uint32_t api_version;
	call = vkEnumerateInstanceVersion(&api_version);
	if (call == VK_SUCCESS)	// else on iOS/Mac this vkFunction doesn't seem defined in MoltenVK
		Log(NOTE, "Supported Vulkan Instance Version: %d", api_version);

	VkApplicationInfo appInfo = {
		.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext				= nullptr,
		.pApplicationName	= AppConstants.AppName,
		.applicationVersion	= AppConstants.AppVersion,
		.pEngineName		= VulkanSingleton::instance().EngineName,
		.engineVersion		= VulkanSingleton::instance().EngineVersion,
		.apiVersion			= TARGET_VULKAN_API_VERSION
	};

	VkInstanceCreateInfo instInfo = {
		.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext					 = nullptr,
		.flags					 = 0,
		.pApplicationInfo		 = &appInfo,
		.enabledLayerCount		 = layers.NumEnabledLayers(),
		.ppEnabledLayerNames	 = layers.ppEnabledLayerNames(),
		.enabledExtensionCount	 = platform.getVulkanExtensionCount(),
		.ppEnabledExtensionNames = platform.getSupportedVulkanExtensions()
	};

	Log(NOTE, "Initializing Vulkan runtime instance...");

	call = vkCreateInstance(&instInfo, nullALLOC, &instance);

	if (call == VK_ERROR_INCOMPATIBLE_DRIVER)
		Fatal("INCOMPATIBLE DRIVER: Unable to create Vulkan instance, cannot find a Vulkan ICD (installable client driver).");
	else if (call != VK_SUCCESS)
		Fatal("Create Vulkan Instance FAILURE" + ErrStr(call));
}
