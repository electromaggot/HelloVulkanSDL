//
// Local VkEnumerateInstanceVersion.cpp
//	Vulkan Module		Apple-centric
//
// For Apple platforms, provide at least an instance of
//	vkEnumerateInstanceVersion that MoltenVK doesn't seem to
//	(and keep a platform-specific macro from polluting common code).
//
// Created 3/2/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
	auto func = (PFN_vkEnumerateInstanceVersion) vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
	if (func) {
		func(pApiVersion);
		return VK_SUCCESS;
	}
	return VK_ERROR_FEATURE_NOT_PRESENT;
}
