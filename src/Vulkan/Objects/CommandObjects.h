//
// CommandObjects.h
//	Vulkan Setup
//
// Encapsulate Command Pool/Buffer/queues.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef CommandObjects_h
#define CommandObjects_h

#include "GraphicsDevice.h"
#include "WindowSurface.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "Framebuffers.h"
#include "GraphicsPipeline.h"
#include "VertexBasedObject.h"
#include "Descriptors.h"


class CommandPool
{
	friend class CommandObjects;
public:
	CommandPool(GraphicsDevice& graphicsDevice);
	~CommandPool();
	VkCommandPool		vkInstance;
private:
	GraphicsDevice&		device;
};

struct AddOns	//TJ_TODO: need to become own separate class?
{
	friend class CommandObjects;
	AddOns(VertexBasedObject& vertexObject, VkCommandPool& commandPool, GraphicsDevice& graphics);
	~AddOns();
	PrimitiveBuffer*	pVertexBuffer	= nullptr;
	PrimitiveBuffer*	pIndexBuffer	= nullptr;
	//UniformBuffer*	pUniformBuffer	= nullptr;	//TJ: not quite sure how to include this, TBD
};


class CommandObjects
{
public:
	CommandObjects(GraphicsPipeline& pipeline, Framebuffers& framebuffers,
				   RenderPass& renderPass, Swapchain& swapchain, GraphicsDevice& graphics,
				   VertexBasedObject& vertexObject, Descriptors* pDescriptors);
	~CommandObjects();

		// MEMBERS
private:
	CommandPool		 commandPool;
	AddOns	 		 addedOn;

	VkCommandBuffer* commandBuffers;
	uint32_t		 nCommandBuffers;

	GraphicsDevice&	 device;

		// METHODS
private:
	void createCommandPool();
	void allocateCommandBuffers();
	void createCommandBuffers(vector<VkFramebuffer>& framebuffers, VkExtent2D& swapChainExtent,
							  VkRenderPass& renderPass, GraphicsPipeline& graphicsPipeline,
							  VertexBasedObject& vertexObject, Descriptors* pDescriptors);
public:
	void Recreate(GraphicsPipeline& pipeline, Framebuffers& framebuffers,
				  RenderPass& renderPass, Swapchain& swapchain,
				  VertexBasedObject& pVertexObject, bool reloadMesh = false,
				  Descriptors* pDescriptors = nullptr);
		// getters
	const VkCommandBuffer*	CommandBuffers()	{	return commandBuffers;	}
	uint32_t				NumCommandBuffers() {	return nCommandBuffers;	}
};

#endif // CommandObjects_h
