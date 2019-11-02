//
// VulkanWindowSurface.h
//	Vulkan Setup
//
// Encapsulate the display surface rendered to, be that full-screen or windowed.
//	Also proxy the platform for window-related operations.
//
// TODO: Eventually support multi-monitor, i.e. unique VkSurfaceKHR
//	instances per possibly-multiple displays/graphicsDevices.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanWindowSurface_h
#define VulkanWindowSurface_h

#include "VulkanInstance.h"
#include "iPlatform.h"
#include <algorithm>


using std::numeric_limits; using std::min; using std::max;


class WindowSurface
{
public:
	WindowSurface(VulkanInstance& vulkan, iPlatform& windowingSystem);
	~WindowSurface();

		// MEMBERS
private:
	VkSurfaceKHR	presentationSurface;

	VkInstance&		instance;	// save, initialization used it, destruction requires it

	iPlatform&		platform;

		// METHOD
private:
	void createSurface(iPlatform& platform);
public:
	VkExtent2D GetWindowExtent();

		// getter
	VkSurfaceKHR& getVkSurface()	{ return presentationSurface; }
};

#endif // VulkanWindowSurface_h
