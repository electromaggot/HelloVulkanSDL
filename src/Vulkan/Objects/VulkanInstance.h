//
// VulkanInstance.h
//	Vulkan Setup
//
// Vulkan module central object; encapsulates VkInstance.
//
//	Accepts a ValidationLayers object used to enable the
//	Instance Layers that Vulkan already reported it supports;
//	teh layers provide debugging, validation, profiling, etc.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanInstance_h
#define VulkanInstance_h

#include "iPlatform.h"
#include "ValidationLayers.h"


class VulkanInstance
{
public:
	VulkanInstance(ValidationLayers& validation, iPlatform& platform);
	~VulkanInstance();

		// MEMBERS
private:
	VkInstance instance;

	bool isDebugReportProvided;

		// METHODS
private:
	void createInstance(iPlatform& platform, ValidationLayers& layers);

		// getters
public:
	VkInstance& getVkInstance()			{ return instance; }
	bool		supportsDebugReport()	{ return isDebugReportProvided; }
};

#endif	// VulkanInstance_h
