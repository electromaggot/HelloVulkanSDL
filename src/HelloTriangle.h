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
//		to specify and load shader files, etc., which the rest of
//		Vulkan's pipeline depends upon.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "AppSettings.h"
#include "PlatformSDL.h"
#include "VulkanSetup.h"


class HelloApplication
{
private:
	PlatformSDL		platform;	// SDL is target "platform" (i/o abstraction layer).

	VulkanSetup		vulkan;		// This one instantiation...

public:
	HelloApplication()
		:	platform(),
			vulkan(platform),	//	...initializes almost all of Vulkan.
			device(vulkan.device.getLogical()),
			swapchain(vulkan.swapchain.getVkSwapchain()),
			deviceQueue(vulkan.device.Queues.getCurrent()),
			syncObjects(vulkan.syncObjects),
			swapchainExtent(vulkan.swapchain.getExtent())
	{ }

	~HelloApplication()			// Before destruction, in turn destroying child
	{								//	data structures instanced above, ensure
		vkDeviceWaitIdle(device);	//	Vulkan finish any operations in-process.
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
	void updateSpinOnZAxis(float time, float aspectRatio);
	void updateRayCast(float time);

	static void ForceUpdateRender(void* pOurself);
};
