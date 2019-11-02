//
// VulkanWindowSurface.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// Note that calling vkGetPhysicalDeviceSurfaceSupportKHR is not necessary
//	here -- as in, to ensure the surface is compatible with the Render Queue
//	and Physical Device, and that Present is supported -- because that's
//	subsequently done in DeviceQueues::DetermineFamilyIndices.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "WindowSurface.h"


WindowSurface::WindowSurface(VulkanInstance& vulkan, iPlatform& windowingSystem)
	:	instance(vulkan.getVkInstance()),
		platform(windowingSystem)
{
	createSurface(platform);
}

WindowSurface::~WindowSurface()
{
	vkDestroySurfaceKHR(instance, presentationSurface, nullALLOC);
}


// Create the Vulkan Surface to render to, by requesting it from the platform, which
//	already made a window for it.
//
void WindowSurface::createSurface(iPlatform& platform)
{
	platform.CreateVulkanSurface(instance, presentationSurface);
}


// Common platform-abstraction layers (SDL, GLFW) return window sizes in 'int's.
//	Cast them into a Vulkan Extent.  If they won't go cleanly, make the Extent
//	appear invalid just as Vulkan does, by assigning max_uint values.
//
VkExtent2D WindowSurface::GetWindowExtent()
{
	int pixelWidth, pixelHeight;
	bool ok = platform.GetWindowSize(pixelWidth, pixelHeight);
	if (ok && pixelWidth > 0 && pixelHeight > 0)
		return { static_cast<uint32_t>(pixelWidth), static_cast<uint32_t>(pixelHeight) };
	const uint32_t INVALID = numeric_limits<uint32_t>::max();
	return { INVALID, INVALID };
}
