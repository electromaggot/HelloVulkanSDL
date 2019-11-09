//
// VulkanSetup.cpp
//	Vulkan Module
//
// See matched header file for definitive main comment.
//
// Note that ValidationLayers and DebugReport are initialized as early as possible
//	to enable and begin, especially initialization-related, debug reporting.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanSetup.h"


VulkanSetup::VulkanSetup(iPlatform& platform)
	:	validation(),							// Initializer list: instantiate components
		vulkan(validation, platform),			//	in ascending order of explicit dependencies.
		debugReport(vulkan),
		windowSurface(vulkan, platform),
		device(windowSurface, vulkan, validation),
		renderPass(device),
		swapchain(device, windowSurface),
		framebuffers(swapchain, renderPass, device),
		syncObjects(device),
		commandPool(device)
{
	pShaderModules	= nullptr;
	pUniformBuffer	= nullptr;	//TJ_CONSIDER: for MVP, could this be made part of above init list?
	pPipeline		= nullptr;
	pCommandObjects	= nullptr;
}

// On the other hand, these later child objects will NOT leave scope and self-destruct,
//	so:
VulkanSetup::~VulkanSetup()
{
	delete pCommandObjects;
	delete pPipeline;
	for (auto& pTextureImage : pTextureImages)
		delete pTextureImage;
	delete pUniformBuffer;
	delete pShaderModules;
}

// pipeline and commandObjects are not in the list above, because they need to be part
//	of a more flexible Rendering component, which initializes later (than "load time"
//	or "allocation time") and passes in runtime aspects like then-loaded shaders.
//	Thus shader loading/unloading is dynamic.
//
void VulkanSetup::PostConstruction_SetUpForRendering(Shaders& shaders, VertexBasedObject& vtxObj,
													 vector<Described> describedAddOns)
{
	pShaderModules = new ShaderModules(shaders, device);

	pDescriptors = new Descriptors(describedAddOns, swapchain, device);

	pPipeline = new GraphicsPipeline(*pShaderModules, renderPass, swapchain, device,
									 &vtxObj.vertexType, pDescriptors);

	pCommandObjects = new CommandObjects(*pPipeline,
										framebuffers, renderPass, swapchain, device,
										vtxObj, pDescriptors);

	pVertexObject = &vtxObj; //TJ_TODO: What this points to better not be freed or changed randomly!
							 //  We're lucky that in this early going, it points to a combination of
							 //  statics/globals, but if this reference ever arrives to a dynamically
							 //  allocated object or one that leaves scope, "not such a good idea."
							 //  Saving this is hokey anyway, only persisting a VertexType for a
							 //	 Recreate that keeps the same Vertex(&Index)Buffer.
							 // This code, AddOns, and 'Recreate's are all in flux as it is, so I don't
							 //  want to prematurely optimize.  It'll probably change then get fixed.
}

// Assemble a collection of Descriptors to be "added on."  Ordering is critical:
//	make sure each INDEX matches its "layout(binding = <INDEX>)" in your Shader.
//																					// e.g. :
vector<Described> VulkanSetup::AddOn(UBO* pUBO, TextureSpec textureSpecs[],
									 iPlatform& platform)
{
	vector<Described> describedItems;

	// Uniform Buffer Objects first (explicitly: the MVP UBO)
	if (pUBO) {
		pUniformBuffer = new UniformBuffer(pUBO->byteSize, swapchain, device);
		shaderStageForUBO = pUBO->getShaderStageFlags();
		describedItems.emplace_back(pUniformBuffer->getDescriptorBufferInfo(),		// layout(binding = 0)
									shaderStageForUBO);
	}

	// Textures next (may be more than one)... order is important here too == binding index
	for (TextureSpec* pTextureSpec = textureSpecs; pTextureSpec && pTextureSpec->fileName; ++pTextureSpec) {
		TextureImage* pTexture = new TextureImage(*pTextureSpec, commandPool.vkInstance, device, platform);
		if (pTexture) {
			pTextureImages.emplace_back(pTexture);
			describedItems.emplace_back(pTexture->getDescriptorImageInfo(),			// layout(binding = 1) ... 2) ... 3)...
										VK_SHADER_STAGE_FRAGMENT_BIT);
										// ^^^^^^ TODO: ^^^^^^^^ We don't have a mechanism (YET!) allowing an image to
		}								//		be specified for the VERTEX STAGE, which could be helpful for something
	}									//		like offseting vertices based on a depth map.
	return describedItems;
}


void VulkanSetup::RecreateRenderingRudiments()
{
	const bool reloadMesh = false;

	vkDeviceWaitIdle(device.getLogical());

	VertexType* pVertexType = pVertexObject ? const_cast<VertexType*>(&pVertexObject->vertexType) : nullptr;

	swapchain.Recreate();
	framebuffers.Recreate(swapchain, renderPass);
	if (pUniformBuffer)
		pUniformBuffer->Recreate(-1, swapchain);
	vector<Described> describedAddOns = reDescribe();
	if (pDescriptors)
		pDescriptors->Recreate(describedAddOns, swapchain);
	pPipeline->Recreate(*pShaderModules, renderPass, swapchain, pVertexType, pDescriptors);
	pCommandObjects->Recreate(*pPipeline, framebuffers, renderPass, swapchain,
							  *pVertexObject, reloadMesh, pDescriptors);
}

// Note that TextureImages are specifically not reloaded or regenerated.
//	This may be necessary later, should the images change or animate, although
//	loading entirely new images (and discarding old ones) is separate matter.
//
vector<Described> VulkanSetup::reDescribe()
{
	vector<Described> redescribedAddOns;
	if (pUniformBuffer)
		redescribedAddOns.emplace_back(pUniformBuffer->getDescriptorBufferInfo(),
									   shaderStageForUBO);
	for (auto& pTextureImage : pTextureImages)
		redescribedAddOns.emplace_back(pTextureImage->getDescriptorImageInfo(),
									   VK_SHADER_STAGE_FRAGMENT_BIT);
	return redescribedAddOns;
}

