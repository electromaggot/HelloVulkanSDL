//
// CommandBufferBase.h
//	Vulkan Add-ons
//
// Base class for shared command buffer operations
//	on top of general buffer operations.
//
// Created 6/29/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef CommandBufferBase_h
#define CommandBufferBase_h

#include "BufferBase.h"


class CommandBufferBase : protected BufferBase
{
protected:
	CommandBufferBase(VkCommandPool& pool, GraphicsDevice& device)
		:	BufferBase(device),
			commandPool(pool),
			graphicsQueue(device.Queues.Graphics())
	{ }

		// MEMBERS
	VkCommandPool&	commandPool;
	VkQueue&		graphicsQueue;

	const uint32_t	nCommandBuffers = 1;

		// METHODS

	VkCommandBuffer beginSingleSubmitCommands()
	{
		VkCommandBuffer	commandBuffer;
		VkCommandBufferAllocateInfo allocInfo = {
			.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext	= nullptr,
			.commandPool		= commandPool,
			.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount	= nCommandBuffers
		};
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {
			.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext	= nullptr,
			.flags	= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	// (i.e. only do this command once)
			.pInheritanceInfo = nullptr
		};
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void endAndSubmitCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		const uint32_t nSubmits = 1;
		VkSubmitInfo submitInfo = {
			.sType	= VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext	= nullptr,
			.waitSemaphoreCount	= 0,
			.pWaitSemaphores	= nullptr,
			.pWaitDstStageMask	= nullptr,
			.commandBufferCount		= nCommandBuffers,
			.pCommandBuffers		= &commandBuffer,
			.signalSemaphoreCount = 0,
			.pSignalSemaphores	  = nullptr
		};
		vkQueueSubmit(graphicsQueue, nSubmits, &submitInfo, VK_NULL_HANDLE);

		vkQueueWaitIdle(graphicsQueue);		// wait until copy finishes! (*)

		vkFreeCommandBuffers(device, commandPool, nCommandBuffers, &commandBuffer);
	}
};

#endif // CommandBufferBase_h
