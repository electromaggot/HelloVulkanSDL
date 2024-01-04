//
// HelloTriangle Application.h
//
// A "hello world"esque "simplest possible" example using Vulkan with SDL
//	to create a window (or go fullscreen on mobile) and return a Vulkan-
//	compatible surface to render to.  Vulkan itself is encapsulated into
//	classes to: create the Vulkan instance, select a graphics device,
//	then set up swapchain, render pass, pipeline, frame buffers, etc.
//
// Note that there are two phases to setting-up Vulkan:
//	1. VulkanSetup's constructor/initializer list (instantiated below)-
//		Creates nearly all of Vulkan's objects, in order of dependency,
//		letting a "resource allocation is initialization" approach
//		manage object lifespans in minimal code.
//	2. Post-construction (once program has the chance to start running)-
//		Finishes initialization that requires some processing, like
//		to specify and load shader files, etc., which the rest of a
//		Vulkan pipeline depends upon.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "AppSettings.h"
#include "PlatformSDL.h"
#include "VulkanSetup.h"
#include "DrawableSpecifier.h"
#include "OtherDrawables.h"
#include "GameClock.h"


const SteerSetup simple = NO_DEPTH_BUFFER;


class HelloApplication
{
private:
	PlatformSDL		platform;			// SDL is target "platform" (i/o abstraction layer).

	VulkanSetup		vulkan;				// This one instantiation...

public:
	HelloApplication()
		:	platform(),
			vulkan(platform, simple),	//		...initializes almost all of Vulkan.
			device(vulkan.device.getLogical()),				//
			swapchain(vulkan.swapchain.getVkSwapchain()),	// Then the rest of this initializer
			deviceQueue(vulkan.device.Queues.getCurrent()),	//	list stores local references to
			syncObjects(vulkan.syncObjects),				//	avoid repeated dereferencing.
			swapchainExtent(vulkan.swapchain.getExtent())	//
	{ }

	~HelloApplication()					// Before destruction, in turn destroying child
	{									//	data structures instanced above, ensure
		vkDeviceWaitIdle(device);		//	Vulkan finish any operations in-process.

		if (pOtherDrawables)
			delete pOtherDrawables;
	}

		// lesser MEMBERS
private:
	int iCurrentFrame = 0;

	// Convenience references used every frame:
	VkDevice&		device;
	VkSwapchainKHR&	swapchain;
	VkQueue&		deviceQueue;
	SyncObjects&	syncObjects;
	VkExtent2D&		swapchainExtent;

	// Draw-loop-related detail
	typedef uint64_t NanosecondTimeout;
	const NanosecondTimeout NO_TIMEOUT = numeric_limits<uint64_t>::max();
	const NanosecondTimeout FAILSAFE_TIMEOUT = 100'000'000;					// 1/10th second
	VkResult call;		// local instance (the global one, while convenient, isn't thread-safe)

	// App-specific
	OtherDrawables*	pOtherDrawables = nullptr;
	GameClock		gameClock;

		// METHODS
public:
	void Run();
	void Init();
	void DialogBox(const char* message) { platform.DialogBox(message); }
private:
	void prepareForMainLoop();
	void updateRender();
	void update();
	void draw();

	void loadRenderable(DrawableSpecifier& specified);
	void loadNextRenderable();

	void recalculateProjectionIfChanged();
	void setPerspectiveProjection();

	static void ForceUpdateRender(void* pOurself);
};
