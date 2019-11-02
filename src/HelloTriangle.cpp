//
// HelloTriangle Application.cpp
//
// See matched header file for definitive main comment.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"
#include "VertexBasedObject.h"
#include "UniformBufferObjects.h"

#include "VertexNull.h"
VertexBasedObject shaderSets3Vertices = {	// Initializing this structure is necessary when
	VertexNull, nullptr,					//	using Shaders that define their own vertices,
	3, 0, 1, 0								//	here, THREE of them, so the Drawing code can
};											//	tell Vulkan how many vertices there are.

extern VertexBasedObject Quad2DTextured;
extern VertexBasedObject Quad2DTextureTinted;
extern VertexBasedObject Quad2DColored;
extern VertexBasedObject Triangle2DColored;

UBO_MVP	MVP;		//TJ_TEMPORARILY_HERE, see if this needs its own identity or to combine with texture buffer
UBO uboMVP(MVP);

UBO_rtm RayCast;
UBO uboRayCast(RayCast);

UBO uboCurrent;

#include <chrono>	//TJ_TEMPORARILY_HERE, this too, it will eventually go where game loop per-frame delta-timing resides


// Actually this "POST-CONSTRUCTION INITIALIZATION" runs after VulkanSetup's
//	initializer list instantiates/initializes nearly all of the Vulkan objects.
//	Now instruct it how to finish, specifying app-specific customizations.
//
void HelloApplication::Init()
{
	const int iMode = 7;

	static Shaders initialShaders[] = {
		{		// 0: full-screen color gradient
			{ VERTEX_SHADER,	"FullScreenTriangle-vert.spv"		},
			{ FRAGMENT_SHADER,	"basicPlaid-frag.spv"				}
		},{		// 1: triangle, shader-defined vertices, colored per vertex
			{ VERTEX_SHADER,	"09_shader_base-vert.spv"			},
			{ FRAGMENT_SHADER,	"09_shader_base-frag.spv"			}
		},{		// 2: triangle, vertex buffer, colored per vertex
			{ VERTEX_SHADER,	"17_shader_vertexbuffer-vert.spv"	},		// these shaders require
			{ FRAGMENT_SHADER,	"17_shader_vertexbuffer-frag.spv"	}		//	a vertex buffer below
		},{		// 3: quad, vertex + index buffer, color per vertex
			{ VERTEX_SHADER,	"17_shader_vertexbuffer-vert.spv"	},
			{ FRAGMENT_SHADER,	"17_shader_vertexbuffer-frag.spv"	}
		},{		// 4: quad in 3D, uniform buffer, color vertex
			{ VERTEX_SHADER,	"21_shader_ubo-vert.spv"			},		// shader requires same quad
			{ FRAGMENT_SHADER,	"21_shader_ubo-frag.spv"			}		//	plus uniform buffer below
		},{		// 5: textured quad with colored vertices
			{ VERTEX_SHADER,	"25_shader_textures-vert.spv"		},		// and now requires
			{ FRAGMENT_SHADER,	"25_shader_textures-frag.spv"		}		//	a texture too
		},{		// 6: minimal textured quad
			{ VERTEX_SHADER,	"TexturedQuad-vert.spv"				},
			{ FRAGMENT_SHADER,	"TexturedQuad-frag.spv"				}
		},{		// 7: ray marching test
			{ VERTEX_SHADER,	"FullScreenTriangle-vert.spv"		},
			{ FRAGMENT_SHADER,	"tjVolcanic-frag.spv"				}
		}
	};
	static VertexBasedObject vertexSpecifiers[] = {
		//		 0					  1					  2				   3
		shaderSets3Vertices, shaderSets3Vertices, Triangle2DColored, Quad2DColored,
		//							  4					5				  6					7
								Quad2DColored, Quad2DTextureTinted, Quad2DTextured, shaderSets3Vertices
	};
	static UBO* pUBOs[] = {
		// 0		1		 2		  3		   4		5		 6			7
		nullptr, nullptr, nullptr, nullptr, &uboMVP, &uboMVP, &uboMVP, &uboRayCast
	};
	const int MAX_ADDED_TEXTURES = 3;	// must match data structure initialized below
	static TextureSpec texSpecs[][MAX_ADDED_TEXTURES + 1] = {	/*
		 0	  1	   2	3	 4						 ^^^ "null terminates" even the longest array */
		{ }, { }, { }, { }, { },
		//		  5						 6
		{ { "texture.jpg" } }, { { "texture.jpg" } },
		//		 7.1								  7.2									7.3
		{ { "Noise256.png", LINEAR, REPEAT }, { "MossyBark.jpg", MIPMAP, REPEAT }, { "PockScorch.jpg", MIPMAP, REPEAT }	}
	};

	if (pUBOs[iMode])
		uboCurrent = *pUBOs[iMode];

	vulkan.PostConstruction_SetUpForRendering(initialShaders[iMode], vertexSpecifiers[iMode],
											  vulkan.AddOn(pUBOs[iMode], texSpecs[iMode], platform));
	prepareForMainLoop();
}


void HelloApplication::Run()
{
	platform.ClearEvents();

	for (bool quit = false; !quit; )
	{
		while (platform.PollEvent())
			quit = platform.IsEventQUIT();

		if (platform.IsWindowMinimized())
			platform.AwaitEvent();

		update();

		draw();
	}
}


void HelloApplication::update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	MVP.model = glm::rotate(mat4(1.0f), time * radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
					// identity matrix		rotate 90 deg/sec		around Z axis
	MVP.proj = glm::perspective(radians(45.0f), swapchainExtent.width / (float) swapchainExtent.height, 0.1f, 10.0f);
								// fov: 45 degrees				aspect ratio							near	far

	//uboMVP.proj[1][1] *= -1;	//TJ_DELETE_LATER: the original tutorial did this too, to flip Y's, and set eye at 2,2,2 and +Z for up

	RayCast.resolution	= vec4(float(platform.pixelsWide), float(platform.pixelsHigh), 1.0f, 0.0f);
	RayCast.time		= time;
	RayCast.mouse		= vec4(float(platform.mouseX), float(platform.mouseY), 0.0f, 0.0f);
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void HelloApplication::prepareForMainLoop()
{
	pCommandObjects	= vulkan.pCommandObjects;
	pUniformBuffer  = vulkan.pUniformBuffer;

	MVP.view = glm::lookAt(vec3(0.0f, -2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
						 // eye: 2 back 2 up		  center @ origin		up: negative Y points up
}

void HelloApplication::draw()
{
	const uint64_t NO_TIMEOUT = numeric_limits<uint64_t>::max();
	uint32_t iImage;

	// Await prior submission's finish...
	vkWaitForFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame], VK_TRUE, NO_TIMEOUT);

	call = vkAcquireNextImageKHR(device, swapchain, NO_TIMEOUT,
								 syncObjects.imageAvailableSemaphores[iCurrentFrame],
								 VK_NULL_HANDLE, &iImage);

	if (call == VK_ERROR_OUT_OF_DATE_KHR)
	{
		vulkan.RecreateRenderingRudiments();
		return;
	}
	else if (call != VK_SUCCESS && call != VK_SUBOPTIMAL_KHR)
		Log(ERROR, "Acquire Next Image" + ErrStr(call));

	//	...then restore Fence back to unsignalled state.
	vkResetFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame]);

	if (pUniformBuffer)
		pUniformBuffer->Update(iImage, uboCurrent.pUBO, uboCurrent.byteSize);

	// SUBMIT --------------------------------------------------------------------------------------

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo = {		// (one submit) (hence the "1" below)
		.sType	= VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext	= nullptr,
		.waitSemaphoreCount	= 1,
		.pWaitSemaphores	= &syncObjects.imageAvailableSemaphores[iCurrentFrame],
		.pWaitDstStageMask		= waitStages,
		.commandBufferCount	= 1,	// don't submit more than one!
		.pCommandBuffers	= &pCommandObjects->CommandBuffers()[iImage],
		.signalSemaphoreCount	= 1,
		.pSignalSemaphores		= &syncObjects.renderFinishedSemaphores[iCurrentFrame]
	};

	call = vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[iCurrentFrame]);

	if (call != VK_SUCCESS)
		Fatal("Queue Submit draw command buffer FAILURE" + ErrStr(call));

	// PRESENT -------------------------------------------------------------------------------------

	VkPresentInfoKHR presentInfo = {
		.sType	= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext	= nullptr,
		.waitSemaphoreCount	= 1,
		.pWaitSemaphores	= &syncObjects.renderFinishedSemaphores[iCurrentFrame],
		.swapchainCount		= 1,
		.pSwapchains		= &swapchain,
		.pImageIndices	= &iImage,
		.pResults		= nullptr
	};

	call = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (platform.IsWindowResized() || call == VK_ERROR_OUT_OF_DATE_KHR
								   || call == VK_SUBOPTIMAL_KHR)
		vulkan.RecreateRenderingRudiments();
	else if (call != VK_SUCCESS)
		Log(ERROR, "Queue Present" + ErrStr(call));

	iCurrentFrame = (iCurrentFrame + 1) % syncObjects.MaxFramesInFlight;
}
