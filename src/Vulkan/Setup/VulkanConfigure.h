//
// VulkanConfigure.h
//	Vulkan Setup
//
// Encapsulate the following, for invocation with Vulkan:
//	- preferred Surface Formats, Color Space, and Present Modes.
//	- required/desired Instance Extensions, Instance Layers, and Device Extensions.
//
// Specify via each REQUIRE_ array whether an Extension or Layer is REQUIRED versus
//	merely "desired," thus if that Extension/Layer is unsupported or unavailable,
//	it fatally throws an exception versus being tolerated (simply ignored & skipped).
//
// All these criteria are part of the Physical Device/GPU selection process.  The
//	"ideal" Device supports all Required and non-required Extensions and Layers.
//	Devices not supporting a Required feature cannot be selected and a Device is
//	preferred over others if it supports more non-required/"desired" features.
//
// 7/15/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanConfigure_h
#define VulkanConfigure_h

#include "VulkanPlatform.h"
#include "PlatformSpecifics.h"


enum DeviceSelectionMethod {
	AUTOMATIC,
	PROMPT_USER,
	SPECIFIED
};
const DeviceSelectionMethod DEVICE_SELECTION_MODE = AUTOMATIC;

const StrPtr DEVICE_NAME_IF_SPECIFIED = "AMD Radeon Pro 560";


// SURFACE FORMAT (pixel color depth, color space) and PRESENTATION MODE

const VkFormat PIXEL_FORMAT_PRECEDENCE[] = {

	VK_FORMAT_B8G8R8A8_UNORM
};
const int N_PIXEL_FORMAT_PRECEDENCES = N_ELEMENTS_IN_ARRAY(PIXEL_FORMAT_PRECEDENCE);

const VkColorSpaceKHR COLOR_SPACE_PRECEDENCE[] = {

	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
};
const int N_COLOR_SPACE_PRECEDENCES = N_ELEMENTS_IN_ARRAY(COLOR_SPACE_PRECEDENCE);

const VkPresentModeKHR PRESENT_MODE_PRECEDENCE[] = {

	VK_PRESENT_MODE_MAILBOX_KHR,
	VK_PRESENT_MODE_IMMEDIATE_KHR,	// this precedes and is supposedly preferable to
	VK_PRESENT_MODE_FIFO_KHR		//	this, which "unfortunately some drivers currently don't properly support"
};
const int N_PRESENT_MODE_PRECEDENCES = N_ELEMENTS_IN_ARRAY(PRESENT_MODE_PRECEDENCE);
const int N_PRECEDENCES = N_PIXEL_FORMAT_PRECEDENCES + N_COLOR_SPACE_PRECEDENCES + N_PRESENT_MODE_PRECEDENCES;


// INSTANCE EXTENSIONS/LAYERS AND DEVICE EXTENSIONS

#define	DEBUG_REPORT_EXTENSION		VK_EXT_DEBUG_REPORT_EXTENSION_NAME

#define STANDARD_VALIDATION_LAYER	"VK_LAYER_LUNARG_standard_validation"

#define SWAPCHAIN_EXTENSION			VK_KHR_SWAPCHAIN_EXTENSION_NAME


const StrPtr INSTANCE_EXTENSION_NAMES[] = {
//			 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
	DEBUG_REPORT_EXTENSION			// Add debug display extension; needed to relay debug messages.
	//TJ_REMOVE_LATER for testing:
	//,VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME	//test: also won't be defined
	//,VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME	//test: but will be
	//,VK_AMD_SHADER_CORE_PROPERTIES_EXTENSION_NAME
	//,VK_NVX_raytracing	// note also requires VK_PIPELINE_BIND_POINT_RAYTRACING_NVX

	//TJ_NOTE - also available:
	//"VK_EXT_debug_utils"
};
const int N_INSTANCE_EXTENSION_NAMES = N_ELEMENTS_IN_ARRAY(INSTANCE_EXTENSION_NAMES);

const bool REQUIRE_INSTANCE_EXTENSION[] = {
	false
};


const StrPtr INSTANCE_LAYER_NAMES[] = {
//			 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯
	STANDARD_VALIDATION_LAYER,
	"VK_LAYER_LUNARG_assistant_layer",
	//"VK_LAYER_NV_optimus"				// Note: example of device layer, here NVIDIA-specific

	//TJ_NOTE - also available:
	//	VK_LAYER_LUNARG_screenshot, VK_LAYER_LUNARG_api_dump, VK_LAYER_LUNARG_device_simulation
};
const int N_INSTANCE_LAYER_NAMES = N_ELEMENTS_IN_ARRAY(INSTANCE_LAYER_NAMES);

const bool REQUIRE_INSTANCE_LAYER[] = {
	false,
	false
};


// Note that Device Extensions, especially if Required, mandate the Device Selection process.

const StrPtr DEVICE_EXTENSION_NAMES[] = {
//			 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
	SWAPCHAIN_EXTENSION
};
const int N_DEVICE_EXTENSION_NAMES = N_ELEMENTS_IN_ARRAY(DEVICE_EXTENSION_NAMES);

const bool REQUIRE_DEVICE_EXTENSION[] = {
	true
};


// some UTILITY METHODS  (like to assist logging & debugging)

#define stringAllRequiredNames(kind)							\
	inline string stringAllREQUIRED_##kind##_NAMES() {			\
		string allnames;										\
		string comma = "";										\
		for (int iKind = 0; iKind < N_##kind##_NAMES; ++iKind)	\
			if (REQUIRE_##kind[iKind]) {						\
				allnames += comma + kind##_NAMES[iKind];		\
				comma = ", ";									\
			}													\
		return allnames;										\
	}
stringAllRequiredNames(INSTANCE_EXTENSION)
stringAllRequiredNames(INSTANCE_LAYER)
stringAllRequiredNames(DEVICE_EXTENSION)


inline void sanityCheckVulkanConfiguration() {
	assert(N_INSTANCE_EXTENSION_NAMES == N_ELEMENTS_IN_ARRAY(REQUIRE_INSTANCE_EXTENSION));
	assert(N_INSTANCE_LAYER_NAMES	  == N_ELEMENTS_IN_ARRAY(REQUIRE_INSTANCE_LAYER)	);
	assert(N_DEVICE_EXTENSION_NAMES	  == N_ELEMENTS_IN_ARRAY(REQUIRE_DEVICE_EXTENSION)	);
}


#endif // VulkanConfigure_h
