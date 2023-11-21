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
#include "MeshObject.h"
#include "UniformBufferLiterals.h"

#include "FixedRenderable.h"
#include "VertexNull.h"

extern MeshObject Quad2DTextured;
extern MeshObject Quad2DTextureTinted;
extern MeshObject Quad2DColored;
extern MeshObject Triangle2DColored;

UBO_MVP	MVP;
UBO uboMVP(MVP);

UBO_rtm RayCast;
UBO uboRayCast(RayCast);


#include <chrono>	// for game loop, and per-frame delta-time updates
using std::chrono::high_resolution_clock; using std::chrono::duration; using std::chrono::seconds;


const int iDemoMode = 6;	// See below for what this numbering means.


// This "Post-construction Initialization" runs after VulkanSetup's initializer list instantiates/initializes
//	nearly all of the Vulkan objects.  Now instruct it how to finish, specifying app-specific customizations.
//
void HelloApplication::Init()
{
	static DrawableProperties demoSpecifiers[] = {
		{												// 0: full-screen color gradient
			.mesh = ShaderSets3Vertices,
			.shaders = { { VERTEX,	 "FullScreenTriangle-vert.spv"		},
						 { FRAGMENT, "basicPlaid-frag.spv"				} }
		},{												// 1: triangle, shader-defined vertices, colored per vertex
			.mesh = ShaderSets3Vertices,
			.shaders = { { VERTEX,	 "09_shader_base-vert.spv"			},		// Simplest example of a renderable.
						 { FRAGMENT, "09_shader_base-frag.spv"			} }
		},{												// 2: triangle, vertex buffer, colored per vertex
			.mesh = Triangle2DColored,											// This vertex buffer required by...
			.shaders = { { VERTEX,	 "17_shader_vertexbuffer-vert.spv"	},		//	these shaders.
						 { FRAGMENT, "17_shader_vertexbuffer-frag.spv"	} }
		},{												// 3: quad, vertex + index buffer, color per vertex
			.mesh = Quad2DColored,
			.shaders = { { VERTEX,	 "17_shader_vertexbuffer-vert.spv"	},
						 { FRAGMENT, "17_shader_vertexbuffer-frag.spv"	} }
		},{												// 4: quad in 3D, uniform buffer, color vertex
			.mesh = Quad2DColored,												// This quad is required by...
			.shaders = { { VERTEX,	 "21_shader_ubo-vert.spv"			},		//	this shader, which also expects...
						 { FRAGMENT, "21_shader_ubo-frag.spv"			} },
			.pUBOs	 = { uboMVP }												//	this uniform buffer.
		},{												// 5: textured quad with colored vertices
			.mesh = Quad2DTextureTinted,
			.shaders  = { { VERTEX,   "25_shader_textures-vert.spv"		},
						  { FRAGMENT, "25_shader_textures-frag.spv"		} },	// and now requires...
			.pUBOs	  = { uboMVP },
			.textures = { { "texture.jpg" } }									//	a texture too (array).
		},{												// 6: minimal textured quad
			.mesh = Quad2DTextured,												// This quad, required by...
			.shaders  = { { VERTEX,   "TexturedQuad-vert.spv"			},		//	this shader, also requires...
						  { FRAGMENT, "TexturedQuad-frag.spv"			} },
			.pUBOs	  = { uboMVP },												//	this model/view/projection,
			.textures = { { "texture.jpg" } }									//	and this texture.
		},{												// 7: ray marching test
			.mesh = ShaderSets3Vertices,
			.shaders  = { { VERTEX,   "FullScreenTriangle-vert.spv"		},
						  { FRAGMENT, "tjVolcanic-frag.spv"				} },
			.pUBOs	  = { uboRayCast },
			.textures =	{ { "Noise256.png", LINEAR, REPEAT },
						  { "MossyBark.jpg", MIPMAP, REPEAT },
						  { "PockScorch.jpg", MIPMAP, REPEAT } }
		}
	};
	DrawableSpecifier demo(demoSpecifiers[iDemoMode]);

	Renderables& renderables = vulkan.command.renderables;

	renderables.Add(FixedRenderable(demo, vulkan, platform));

	vulkan.command.PostInitPrepBuffers(vulkan);

	prepareForMainLoop();

	platform.RegisterForceRenderCallback(HelloApplication::ForceUpdateRender, this);
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

		updateRender();
	}
}

void HelloApplication::updateRender()
{
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
			pSelf->vulkan.RecreateRenderingRudiments();

		pSelf->updateRender();
	}
}

// "Step forward" all active game elements (e.g. those that move)
//	according to the time that has passed since the previous step.
//
void HelloApplication::update()
{
	static auto startTime = high_resolution_clock::now();
	static float previousSeconds = 0.0f;

	auto currentTime = high_resolution_clock::now();
	float secondsElapsed = duration<float, seconds::period>(currentTime - startTime).count();
	float deltaSeconds = secondsElapsed - previousSeconds;
	previousSeconds = secondsElapsed;

	vulkan.command.renderables.Update(deltaSeconds);

	if (iDemoMode < 7)
		updateSpinOnZAxis(secondsElapsed, swapchainExtent.width / (float) swapchainExtent.height);
	else
		updateRayCast(secondsElapsed);
}

void HelloApplication::updateSpinOnZAxis(float time, float aspectRatio)
{
	MVP.model = glm::rotate(mat4(1.0f), time * radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
					// identity matrix		rotate 90 deg/sec		around Z axis
	MVP.proj = glm::perspective(radians(45.0f), aspectRatio, 0.1f, 10.0f);
								// fov: 45 degrees			 near	far

	//uboMVP.proj[1][1] *= -1;	//TJ_DELETE_LATER: the original tutorial did this too, to flip Y's, and set eye at 2,2,2 and +Z for up
}

void HelloApplication::updateRayCast(float time)
{
	RayCast.resolution	= vec4(float(platform.pixelsWide), float(platform.pixelsHigh), 1.0f, 0.0f);
	RayCast.time		= time;
	RayCast.mouse		= vec4(float(platform.mouseX), float(platform.mouseY), 0.0f, 0.0f);
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void HelloApplication::prepareForMainLoop()
{
	MVP.view = glm::lookAt(vec3(0.0f, -2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
						 // eye: 2 back 2 up		  center @ origin		up: negative Y points up
}


// Render all in-game elements.
//
void HelloApplication::draw()
{
	const uint64_t NO_TIMEOUT = numeric_limits<uint64_t>::max();
	uint32_t iNextImage;

	// Await prior submission's finish...
	vkWaitForFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame], VK_TRUE, NO_TIMEOUT);

	call = vkAcquireNextImageKHR(device, swapchain, NO_TIMEOUT,
								 syncObjects.imageAvailableSemaphores[iCurrentFrame],
								 VK_NULL_HANDLE, &iNextImage);
	const char* called = "Acquire Next Image";

	if (platform.IsWindowResized() || call == VK_ERROR_OUT_OF_DATE_KHR || call == VK_SUBOPTIMAL_KHR)
		vulkan.RecreateRenderingRudiments();

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
		if (platform.IsWindowResized() || call == VK_ERROR_OUT_OF_DATE_KHR || call == VK_SUBOPTIMAL_KHR)
			vulkan.RecreateRenderingRudiments();
	}
	if (call != VK_SUCCESS)
		Log(ERROR, called + ErrStr(call));

	iCurrentFrame = (iCurrentFrame + 1) % syncObjects.MaxFramesInFlight;
}
