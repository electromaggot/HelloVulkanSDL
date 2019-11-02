//
// DebugReport.h
//	subordinate to both ValidationLayers and PlatformExtension
//
// Depends on: VK_LAYER_LUNARG_standard_validation
//		  and: VK_EXT_DEBUG_REPORT_EXTENSION_NAME
// That is, requires support at both layer and extension levels.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DebugReport_h
#define DebugReport_h

#include "VulkanInstance.h"


class DebugReport
{		// XSTRUCTORS
private:
	DebugReport();
public:
	DebugReport(VulkanInstance& vulkan);
	~DebugReport();

		// MEMBERS
private:
	VkDebugUtilsMessengerEXT debugMessenger;

	VkDebugReportCallbackEXT reportCallback;
	//TJ_TODO: above needs some kind of call like: setupDebugCallback(instance, callback);
	//			and also what about?:  setupDebugMessenger();  ???
	//TJ_TODO_2: supposedly DebugUtilsMessenger is new, DebugReportCallback is old.

	VkInstance instance;	// Save same for destruction as used for construction

		// METHODS
private:
	// Debug Utils Messenger

	void setupMessenger();

	VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo);
	void destroyDebugUtilsMessengerEXT();

	//TJ_TODO: Figure out how this call signature differs from other debugReportCallback()
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																 VkDebugUtilsMessageTypeFlagsEXT messageType,
																 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
																 void* pUserData);
	// Debug Report Callback

	void setupReport();

	VkResult createDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
	void destroyDebugReportCallbackEXT();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(VkDebugReportFlagsEXT flags,
															  VkDebugReportObjectTypeEXT objType,
															  uint64_t obj,
															  size_t location,
															  int32_t code,
															  const char* pLayerPrefix,
															  const char* pMsg,
															  void* pUserData);
};

#endif // DebugReport_h
