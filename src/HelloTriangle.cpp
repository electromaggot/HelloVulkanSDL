//
// HelloTriangle Application.cpp
//
// See header file comment for overview.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"

#include "AppConstants.h"
#include "CommandObjects.h"
#include "Renderable.h"
#include "VertexNull.h"


// Hand VulkanModule the settings it needs.  VulkanConfig.h DECLARES this; every application
//	defines it.  The mapping lives here because AppConstants is the application's.  A
//	function-local static, so it resolves correctly whenever first asked -- including during
//	static initialization, before main() runs, which is exactly when AppSettings' constructor
//	asks (via FileSystem::AppLocalStorageDirectory) for the company/project names below.
//	That works because AppConstants declares its Settings member LAST, so every StrPtr it
//	reads here is already initialized by then.
//
const VulkanConfig& AppVulkanConfig()
{
	static VulkanConfig config = [] {
		VulkanConfig cfg;
		cfg.appName				= AppConstants.AppName;
		cfg.appVersion			= AppConstants.AppVersion;
		cfg.companyName			= AppConstants.CompanyName;
		cfg.projectName			= AppConstants.ProjectName;
		cfg.debugLogFileName	= AppConstants.DebugLogFileName;
		// (exePath is NOT set here -- see below; it isn't known yet at static-init time.)
		cfg.windowTitle			= AppConstants.WindowTitle;
		cfg.defaultWindowWidth	= AppConstants.DefaultWindowWidth;
		cfg.defaultWindowHeight	= AppConstants.DefaultWindowHeight;
		cfg.clearColor			= AppConstants.DefaultClearColor;
		cfg.supportStereo3D		= AppConstants.SupportStereo3D;
		return cfg;
	}();

	StrPtr exe = AppConstants.getExePath();		// Refreshed per call, not snapshotted: main()
	config.exePath = exe ? exe : "";			//	sets it from argv[0], which is later than the
												//	first call above (static init).  Never null --
	return config;								//	it reaches printf in LogStartup().
}


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
	vulkan.command.renderables.Add(Renderable(specified, vulkan, platform));

	vulkan.command.PostInitPrepBuffers(vulkan);
}

void HelloApplication::loadNextRenderable()
{
	if (!pOtherDrawables)
		pOtherDrawables = new OtherDrawables(platform);

	vkDeviceWaitIdle(device);
	vulkan.command.RecreateBuffers(vulkan.framebuffers);
	vulkan.command.renderables.Clear();	 // (blanket-clears all of 'em, since in this demo
										 //	 application, only one is active at a time anyway)
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

	//MVP.proj[1][1] *= -1;	// Deliberately NOT the standard Vulkan Y-flip -- see DEV NOTE at
							//	the end of this file, which explains why this app must not use it.
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void HelloApplication::prepareForMainLoop()
{
	const vec3 eyePosition = vec3(0.0f, -2.0f, 2.0f);	// 2 back 2 up
	const vec3 stareAtOrigin = vec3(0.0f, 0.0f, 0.0f);
	const vec3 upVector = vec3(0.0f, -1.0f, 0.0f);		// negative Y points up (see DEV NOTE)

	MVP.view = glm::lookAt(eyePosition, stareAtOrigin, upVector);

	setPerspectiveProjection();
}

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
	// Delegate swapchain recreation to the reusable VulkanModule helper.  (This demo has no
	//	per-image tracking to reset; device loss just logs and quits -- recovery is app policy.)
	switch (vulkan.RecoverFromPresentResult(call))
	{
		case FrameRecovery::Recreated:
			break;
		case FrameRecovery::DeviceLost:
			Log(ERROR, "Vulkan device lost - exiting (this demo has no recovery policy).");
			break;
		case FrameRecovery::None:
			if (call != VK_SUCCESS && call != VK_SUBOPTIMAL_KHR)
				Log(ERROR, called + ErrStr(call));
			break;
	}

	iCurrentFrame = (iCurrentFrame + 1) % syncObjects.MaxFramesInFlight;
}


/* DEV NOTE - why this app must NOT use the usual `proj[1][1] *= -1` Y-flip

	Most Vulkan tutorials reconcile GLM's OpenGL-convention clip space (+Y up) with Vulkan's
	(+Y down) by negating the projection's Y: `proj[1][1] *= -1`.  This app instead hands
	glm::lookAt() an inverted up vector, (0,-1,0), and leaves the projection alone.  That looks
	like an oddity to be "corrected."  It is not -- correcting it breaks the demos.  Why:

	  - `proj[1][1] *= -1` negates Y alone.  That is a MIRROR (determinant -1), so it REVERSES
		apparent triangle winding.
	  - Inverting `up` negates both view basis vectors s and u (since s = f x up, u = s x f).
		That is a 180-degree ROTATION (determinant +1), so winding is PRESERVED.

	Winding must be preserved here, because the meshes in graphics/meshObjects/ are shared
	between demos that apply MVP (the UBO quad, the textured quads) and demos that pass their
	vertices straight through to NDC untransformed (the plain vertex-buffer triangle and quad).
	One pipeline, one VkFrontFace, serves both.  A mirror in the projection would flip only the
	transformed half, so whichever half you satisfied, the other would be silently culled.

	Verified empirically rather than by convention: with vertices taken straight to NDC, the
	demos that visibly render produce a NEGATIVE framebuffer-space signed area.  The MVP demos
	produce a negative area too under the arrangement above, and a positive one -- i.e. they
	become back-facing and vanish -- if the projection flip is introduced.
*/
