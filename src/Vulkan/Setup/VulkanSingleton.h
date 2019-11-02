//
// VulkanSingleton.h
//	Vulkan Setup
//
// ...
//
// 2/1/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanSingleton_h
#define VulkanSingleton_h

#include "AppConstants.h"	// (one of just two tie-ins from our Vulkan module back to the App)


class VulkanSingleton
{
public:
	const char* EngineName		 = "None";		// (would perhaps have special significance for an
	const uint32_t EngineVersion = VK_MAKE_VERSION(0, 0, 0);	// engine like unreal/unity/etc.)

	VkClearValue ClearColor		 = AppConstants.DefaultClearColor;

private:
	uint32_t windowWide = static_cast<uint32_t>(AppConstants.DefaultWindowWidth);
	uint32_t windowHigh = static_cast<uint32_t>(AppConstants.DefaultWindowHeight);

// getters
public:
	uint32_t getWindowWidth()	{ return windowWide; }
	uint32_t getWindowHeight()	{ return windowHigh; }


// CONSTRUCTION & INSTANCING
private:										// Make external construction impossible:
	VulkanSingleton() { }
	VulkanSingleton(VulkanSingleton const& copy);				// and copy-constructors
	VulkanSingleton& operator = (VulkanSingleton const& copy);	//	non-implemented too.
public:
	static VulkanSingleton& instance()			// The only instance.
	{											// Guaranteed to be lazy-initialized,
		static VulkanSingleton instance;						// and destroyed correctly.
		return instance;
	}
};

#endif // VulkanSingleton_h
