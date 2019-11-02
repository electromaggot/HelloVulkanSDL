//
// DebugReceiver.cpp
//	subordinate to both ValidationLayers and PlatformExtension
//
// See matched header file for definitive main comment.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "DebugReport.h"


DebugReport::DebugReport(VulkanInstance& vulkan)
	:	instance(vulkan.getVkInstance())
{
	if (vulkan.supportsDebugReport())
	{
		setupMessenger();
		setupReport();
	}
}

DebugReport::~DebugReport()
{
	destroyDebugUtilsMessengerEXT();
	destroyDebugReportCallbackEXT();
}


// Debug Utils Messenger ---------------------------------------------------------------------------

void DebugReport::setupMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
					   //| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
						 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
						 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
					 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
					 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugMessengerCallback,
		.pUserData = nullptr
	};

	if (createDebugUtilsMessengerEXT(&createInfo) != VK_SUCCESS)
		Log(WARN, "Failed to set up debug messenger! (non-fatal)");
}

VkResult DebugReport::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo)
{
	if (auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"))
		return func(instance, pCreateInfo, nullALLOC, &debugMessenger);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DebugReport::destroyDebugUtilsMessengerEXT()
{
	if (auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))
		func(instance, debugMessenger, nullALLOC);
}


VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport::debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
													void* pUserData)
{
	Log(NOTE, "validation layer: %s", pCallbackData->pMessage);
	return VK_FALSE;	// abort the command that produced this message?
}

//TJ_TODO: REMOVE THE ABOVE, THEY'RE NOT BEING USED/CALLED, WHILE THE BELOW ARE.


// Debug Report Callback ---------------------------------------------------------------------------

void DebugReport::setupReport()
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
		.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
			 //| VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			   | VK_DEBUG_REPORT_WARNING_BIT_EXT,
		.pfnCallback = debugReportCallback,
		.pUserData = nullptr
	};

	if (createDebugReportCallbackEXT(&createInfo) != VK_SUCCESS)
		Log(WARN, "Unable to create debug report callback extension.");
}

VkResult DebugReport::createDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo)
{
	if (auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"))
		return func(instance, pCreateInfo, nullALLOC, &reportCallback);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DebugReport::destroyDebugReportCallbackEXT()
{
	if (auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"))
		func(instance, reportCallback, nullALLOC);
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport::debugReportCallback(VkDebugReportFlagsEXT flags,
													VkDebugReportObjectTypeEXT objType,
													uint64_t obj,
													size_t location,
													int32_t code,
													const char* layerPrefix,
													const char* msg,
													void* userData)
{
	Log(RAW, "%s:%s", layerPrefix, msg);
	return VK_FALSE;
}
