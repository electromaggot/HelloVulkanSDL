//
// HelloTriangle Application.cpp
//
// See header file comment for overview.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"

#include "CommandObjects.h"
#include "FixedRenderable.h"
#include "VertexNull.h"


UBO_MVP	MVP;
UBO uboMVP(MVP);


// This "Post-construction Initialization" runs after VulkanSetup's initializer list instantiates/initializes
//	nearly all of the Vulkan objects.  Now instruct it how to finish, specifying app-specific customizations.
//
void HelloApplication::Init()
{
	DrawableProperties demoSpecifier = {		// triangle, shader-defined vertices, colored per vertex
		.mesh = ShaderSets3Vertices,
		.shaders = { { VERTEX,	 "09_shader_base-vert.spv"	},		// Simplest example of a renderable.
					 { FRAGMENT, "09_shader_base-frag.spv"	} },
		.name = "Hello-triangle!"
	};
	DrawableSpecifier demo(demoSpecifier);

	loadRenderable(demo);

	prepareForMainLoop();

	platform.RegisterForceRenderCallback(HelloApplication::ForceUpdateRender, this);
}

void HelloApplication::loadRenderable(DrawableSpecifier& specified)
{
	vulkan.command.renderables.Add(FixedRenderable(specified, vulkan, platform));

	vulkan.command.PostInitPrepBuffers(vulkan);
}

void HelloApplication::loadNextRenderable()
{
	if (!pOtherDrawables)
		pOtherDrawables = new OtherDrawables(platform);

	vkDeviceWaitIdle(device);
	vulkan.command.RecreateBuffers(vulkan.framebuffers);
	vulkan.command.renderables.Clear();	 // (blanket-clears all of 'em, as only one is active at a time anyway)

	DrawableSpecifier nextDrawable(pOtherDrawables->GetNextDrawable());
	loadRenderable(nextDrawable);
}


void HelloApplication::Run()
{
	platform.ClearEvents();

	for (bool quit = false; !quit; )
	{
		while (platform.PollEvent())
			quit = platform.IsEventQUIT();

		if (platform.IsWindowMinimizedOrHidden())
			platform.AwaitEvent();

		if (platform.WasSimplePress())
			loadNextRenderable();

		updateRender();
	}
}

void HelloApplication::updateRender()
{
	gameClock.BeginNewFrame();
	update();
	draw();
}

// Let an external caller force a redraw.  One example: "live window resizing" or pseudorealtime rendering while dragging:
//	an operation that otherwise fully blocks, but may have a Watcher applied that gets called per each change in dimension.
//
void HelloApplication::ForceUpdateRender(void* pOurself)
{
	HelloApplication* pSelf = static_cast<HelloApplication*>(pOurself);
	if (pSelf) {
		if (pSelf->platform.isWindowResized)
			pSelf->vulkan.RecreateRenderingResources();

		pSelf->updateRender();
	}
}

// "Step forward" all active game elements (e.g. those that move)
//	according to the time that has passed since the previous step.
//
void HelloApplication::update()
{
	if (vulkan.command.renderables.Update(gameClock))
		recalculateProjectionIfChanged();
}

void HelloApplication::recalculateProjectionIfChanged()
{
	if (swapchainExtent.width != platform.LastSavedPixelsWide || swapchainExtent.height != platform.LastSavedPixelsHigh)
		setPerspectiveProjection();
}

void HelloApplication::setPerspectiveProjection()
{
	const float nearPlane = 0.1f;
	const float farPlane = 100.0f;

	const float degreeLandscapeFOV = 45.0f;
	const float landscapeVerticalFOV = radians(degreeLandscapeFOV);

	float aspectRatio = swapchainExtent.width / (float) swapchainExtent.height;
	bool isMobileInPortraitMode = (platform.IsMobile && aspectRatio < 1.0f);

	float fieldOfView = landscapeVerticalFOV;
	if (isMobileInPortraitMode)
		fieldOfView = 2.0f * atanf(tanf(landscapeVerticalFOV / 2.0f) / aspectRatio);

	MVP.proj = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);

	//uboMVP.proj[1][1] *= -1;	//note: the original tutorial did this too, to flip Y's, and set eye at 2,2,2 and +Z for up
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void HelloApplication::prepareForMainLoop()
{
	const vec3 eyePosition = vec3(0.0f, -2.0f, 2.0f);	// 2 back 2 up
	const vec3 stareAtOrigin = vec3(0.0f, 0.0f, 0.0f);
	const vec3 upVector = vec3(0.0f, -1.0f, 0.0f);		// negative Y points up

	MVP.view = glm::lookAt(eyePosition, stareAtOrigin, upVector);

	setPerspectiveProjection();
}


// Render all in-game elements.
//
void HelloApplication::draw()
{
	uint32_t iNextImage;

	// Await prior submission's finish...						(and to never risk deadlock ↓ )
	vkWaitForFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame], VK_TRUE, FAILSAFE_TIMEOUT);

	call = vkAcquireNextImageKHR(device, swapchain, FAILSAFE_TIMEOUT,
								 syncObjects.imageAvailableSemaphores[iCurrentFrame],
								 VK_NULL_HANDLE, &iNextImage);
	const char* called = "Acquire Next Image";

	//	...then restore Fence back to unsignaled state.
	vkResetFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame]);

	if (call == VK_SUCCESS)
	{
		vulkan.command.RecordRenderablesForNextFrame(vulkan, iNextImage);

		vulkan.command.renderables.UpdateUniformBuffers(iNextImage);

		// SUBMIT --------------------------------------------------------------------------------------

		VkPipelineStageFlags waitStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		vector<VkCommandBuffer> allRenderablesCommandBuffers = vulkan.command.BuffersForFrame(iNextImage);

		VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &syncObjects.imageAvailableSemaphores[iCurrentFrame],
			.pWaitDstStageMask = &waitStageFlags,
			.commandBufferCount = (uint32_t) allRenderablesCommandBuffers.size(),
			.pCommandBuffers = allRenderablesCommandBuffers.data(),
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &syncObjects.renderFinishedSemaphores[iCurrentFrame]
		};
		VkSubmitInfo submits[] = { submitInfo };
		int numSubmits = N_ELEMENTS_IN_ARRAY(submits);

		call = vkQueueSubmit(deviceQueue, numSubmits, submits, syncObjects.inFlightFences[iCurrentFrame]);
		called = "Queue Submit draw command buffer FAILURE";

		if (call == VK_SUCCESS)
		{
			// PRESENT -------------------------------------------------------------------------------------

			VkPresentInfoKHR presentInfo = {
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext = nullptr,
				.waitSemaphoreCount = 1,
				.pWaitSemaphores = &syncObjects.renderFinishedSemaphores[iCurrentFrame],
				.swapchainCount = 1,
				.pSwapchains = &swapchain,
				.pImageIndices = &iNextImage,
				.pResults = nullptr
			};

			call = vkQueuePresentKHR(deviceQueue, &presentInfo);
			called = "Queue Present";
		}
	}
	if (call == VK_ERROR_OUT_OF_DATE_KHR || call == VK_SUBOPTIMAL_KHR)
	{
		vulkan.RecreateRenderingResources();
		syncObjects.Recreate();
	}
	if (call != VK_SUCCESS && call != VK_SUBOPTIMAL_KHR)
		Log(ERROR, called + ErrStr(call));

	iCurrentFrame = (iCurrentFrame + 1) % syncObjects.MaxFramesInFlight;
}
