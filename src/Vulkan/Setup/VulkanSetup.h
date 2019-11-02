//
// VulkanVars.h
//	Vulkan Setup
//
// RAII (resource allocation is initialization) approach to setting-up
//	Vulkan's various components encapsulated in objects that initialize on
//	construction, incrementally accept dependencies, and release on destruction.
//
// 2/1/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanSetup_h
#define VulkanSetup_h

#define INSTANTIATE

#include "iPlatform.h"
#include "VulkanInstance.h"
#include "ValidationLayers.h"
#include "DebugReport.h"
#include "GraphicsDevice.h"
#include "WindowSurface.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "Framebuffers.h"
#include "SyncObjects.h"

#include "GraphicsPipeline.h"
#include "CommandObjects.h"

#include "VertexBasedObject.h"
#include "UniformBufferObjects.h"
#include "UniformBuffer.h"
#include "TextureImage.h"
#include "Descriptors.h"


class VulkanSetup
{
public:
	VulkanSetup(iPlatform& platform);
	~VulkanSetup();

	// Also public, as owner should have access to these:
	//	(literal "order of appearance" is significant)

	ValidationLayers	validation;
	VulkanInstance		vulkan;
	DebugReport	 		debugReport;
	WindowSurface		windowSurface;
	GraphicsDevice		device;
	RenderPass			renderPass;
	Swapchain			swapchain;
	Framebuffers		framebuffers;
	SyncObjects			syncObjects;
	CommandPool			commandPool;

	ShaderModules*		pShaderModules;
	UniformBuffer*		pUniformBuffer;
	vector<TextureImage*> pTextureImages;
	Descriptors*		pDescriptors;
	GraphicsPipeline*	pPipeline;
	CommandObjects*		pCommandObjects;

	VertexBasedObject*	pVertexObject;		// (retain for Recreate)
	VkShaderStageFlags	shaderStageForUBO;	// ((kludgey) also retain for Recreate)

	void PostConstruction_SetUpForRendering(Shaders& shaders, VertexBasedObject& vertexObject,
											vector<Described> describedAddOns);
	vector<Described> AddOn(UBO* pUBO, TextureSpec textureSpecs[], iPlatform& platform);	// IMPORTANT:
													// ^^^^^^^^^^ This "array" is really just a pointer, and is expected to either be null (nullptr) or point
	void RecreateRenderingRudiments();				//			  to an array of TextureSpec structures TERMINATED by one that is null or having: .fileName == nullptr
private:
	vector<Described> reDescribe();
};

#endif	// VulkanSetup_h



//TJ: I'm still unsure of how FrameBuffers should more-cleanly exist as a child of Pipeline.

//TJ: Below are some legacy "notes" as I cleaned this code up.  Delete later, of course.
/*
	triangle.pickPhysicalDevice();		// GraphicsDevice
	triangle.createLogicalDevice();
	triangle.createSwapChain();			// SwapChain
	triangle.createImageViews();
	triangle.createRenderPass();		// RenderPass
	triangle.createGraphicsPipeline();	// Pipeline
	triangle.createFramebuffers();		// FrameBuffers
	triangle.createCommandPool();		// CommandObjects
	triangle.createCommandBuffers();
	triangle.createSyncObjects();		// SyncObjects
*/
	// Looking at the above and HelloTriangleApplication::cleanup(), new objects justified:
	// SyncObjects
	// CommandObjects (Buffers and Pool)
	// FrameBuffers
	// GraphicsPipeline
	// ?RenderPass?
	// SwapChain (includes ImageViews)
	// Devices (Logical and Physical, and GPU)
	// Surface
	// ValidationLayers (and DebugMessenger)
	// Instance


/* FINALLY, vulkansdldemo main.cpp did this:  (Though poorly. Maybe I need similar progress.)
	std::cout << "\nsuccessfully initialized vulkan and physical device (gpu).\n";
	std::cout << "successfully created a window and compatible surface\n";
	std::cout << "successfully created swapchain\n";
	std::cout << "ready to render!\n";
*/
