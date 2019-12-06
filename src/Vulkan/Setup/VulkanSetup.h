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
