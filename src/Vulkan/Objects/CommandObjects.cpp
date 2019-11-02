//
// CommandObjects.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "CommandObjects.h"
#include "VulkanSingleton.h"
#include "PrimitiveBuffer.h"


CommandObjects::CommandObjects(GraphicsPipeline& pipeline, Framebuffers& framebuffers,
							   RenderPass& renderPass, Swapchain& swapchain, GraphicsDevice& graphics,
							   VertexBasedObject& vertexObject, Descriptors* pDescriptors)
	:	commandPool(graphics),
		addedOn(vertexObject, commandPool.vkInstance, graphics),
		commandBuffers(nullptr),
		device(graphics)
{
	createCommandBuffers(framebuffers.getVkFramebuffers(), swapchain.getExtent(),
						 renderPass.getVkRenderPass(), pipeline,
						 vertexObject, pDescriptors);
}

CommandObjects::~CommandObjects()
{
	delete commandBuffers;	// (also no need to vkFreeCommandBuffers, as vkDestroyCommandPool will)
}


CommandPool::CommandPool(GraphicsDevice& graphicsDevice)
	:	device(graphicsDevice)
{
	VkCommandPoolCreateInfo poolInfo = {
		.sType	= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0,
		.queueFamilyIndex  = device.Queues.GraphicsIndex()
	};

	call = vkCreateCommandPool(device.getLogical(), &poolInfo, nullALLOC, &vkInstance);

	if (call != VK_SUCCESS)
		Fatal("Create Command Pool FAILURE" + ErrStr(call));
}
CommandPool::~CommandPool()
{
	vkDestroyCommandPool(device.getLogical(), vkInstance, nullALLOC);
}


AddOns::AddOns(VertexBasedObject& vertexObject, VkCommandPool& commandPool, GraphicsDevice& graphics)
{
	if (vertexObject.vertices) {
		pVertexBuffer = new PrimitiveBuffer(vertexObject, commandPool, graphics);
		if (vertexObject.indices)
			pIndexBuffer = new PrimitiveBuffer((IndexBufferIndexType*) vertexObject.indices,
											   vertexObject.indexCount, commandPool, graphics);
	}
}						//TJ_TODO: AddOns may eventually need its own file in the AddOns directory.
AddOns::~AddOns()				//	Also with a Recreate routine independent from these (and separate
{								//	Create/Destroy methods not really needed or useful).
	delete pVertexBuffer;
	delete pIndexBuffer;
}


// CommandObjects IMPLEMENTATION

void CommandObjects::allocateCommandBuffers()
{
	commandBuffers = new VkCommandBuffer[nCommandBuffers];

	VkCommandBufferAllocateInfo allocInfo = {
		.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext	= nullptr,
		.commandPool		= commandPool.vkInstance,
		.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = nCommandBuffers
	};

	call = vkAllocateCommandBuffers(device.getLogical(), &allocInfo, commandBuffers);

	if (call != VK_SUCCESS)
		Fatal("Allocate Command Buffers FAILURE" + ErrStr(call));
}

void CommandObjects::createCommandBuffers(vector<VkFramebuffer>& framebuffers, VkExtent2D& swapchainExtent,
										  VkRenderPass& renderPass, GraphicsPipeline& graphicsPipeline,
										  VertexBasedObject& vertexObject, Descriptors* pDescriptors)
{
	nCommandBuffers = (uint32_t) framebuffers.size();

	allocateCommandBuffers();

	VkCommandBufferBeginInfo beginInfo = {
		.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext	= nullptr,
		.flags	= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = nullptr
	};

	VkRenderPassBeginInfo renderPassInfo = {
		.sType	= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.pNext	= nullptr,
		.renderPass		 = renderPass,
		.renderArea		 = { { 0, 0 }, swapchainExtent },
		.clearValueCount = 1,
		.pClearValues	 = &VulkanSingleton::instance().ClearColor
	};

	for (int iBuffer = 0; iBuffer < nCommandBuffers; ++iBuffer)
	{
		VkCommandBuffer& commandBuffer = commandBuffers[iBuffer];

		call = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		if (call != VK_SUCCESS)
			Fatal("Fail to even Begin recording Command Buffer," + ErrStr(call));

		renderPassInfo.framebuffer	= framebuffers[iBuffer];

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.getVkPipeline());

			if (pDescriptors) // if (addedOn.pUniformBuffer)
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
										graphicsPipeline.getPipelineLayout(), 0, 1,
										&pDescriptors->getDescriptorSets()[iBuffer], 0, nullptr);

			if (addedOn.pVertexBuffer) {
				VkBuffer vertexBuffers[] = { addedOn.pVertexBuffer->getVkBuffer() };
				VkDeviceSize offsets[]	 = { 0 };

				vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			}
			if (addedOn.pIndexBuffer) {
				vkCmdBindIndexBuffer(commandBuffer, addedOn.pIndexBuffer->getVkBuffer(),
									 0, IndexBufferIndexTypeEnum);

				vkCmdDrawIndexed(commandBuffer, vertexObject.indexCount, vertexObject.instanceCount,
												vertexObject.firstIndex, vertexObject.vertexOffset,
												vertexObject.firstInstance);
			} else
				vkCmdDraw(commandBuffer, vertexObject.vertexCount, vertexObject.instanceCount,
										 vertexObject.firstVertex, vertexObject.firstInstance);

		vkCmdEndRenderPass(commandBuffer);

		call = vkEndCommandBuffer(commandBuffer);
		if (call != VK_SUCCESS)
			Fatal("Fail to record (End) Command Buffer," + ErrStr(call));
	}
}

// If pVertexObject null, the Vertex/Index Buffers will not be reloaded, but the same ones
//	reused when CommandBuffers recreate.  Otherwise, pass a pointer to one to reload with,
//	or a VertexBaseObject with .vertices = nullptr to eliminate the Vertex Buffer altogether
//	(for instance if vertices are handled in-shader).
//
void CommandObjects::Recreate(GraphicsPipeline& pipeline, Framebuffers& framebuffers,
							  RenderPass& renderPass, Swapchain& swapchain,
							  VertexBasedObject& vertexObject, bool reloadMesh,
							  Descriptors* pDescriptors)
{
	vkFreeCommandBuffers(device.getLogical(), commandPool.vkInstance, nCommandBuffers, commandBuffers);

	delete commandBuffers;					// (note that commandPool can stay as-is)

	if (reloadMesh) {		// otherwise keep the same VertexBuffer we already have loaded
		delete addedOn.pVertexBuffer;
		if (vertexObject.vertices)
			addedOn.pVertexBuffer = new PrimitiveBuffer(vertexObject, commandPool.vkInstance, device);
		else
			addedOn.pVertexBuffer = nullptr;
		delete addedOn.pIndexBuffer;
		if (vertexObject.indices)
			addedOn.pIndexBuffer = new PrimitiveBuffer((IndexBufferIndexType*) vertexObject.indices,
																			   vertexObject.indexCount,
													   commandPool.vkInstance, device);
		else
			addedOn.pIndexBuffer = nullptr;
	}

	createCommandBuffers(framebuffers.getVkFramebuffers(), swapchain.getExtent(),
						 renderPass.getVkRenderPass(), pipeline,
						 vertexObject, pDescriptors);
}
