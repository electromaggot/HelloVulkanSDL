//
// UniformBuffer.h
//	Vulkan Add-ons
//
// Encapsulate Uniform Buffer Objects: their buffers and memory.
//	These rely on the separate Descriptors (pool, sets, layout) class.
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef UniformBuffer_h
#define UniformBuffer_h

#include "BufferBase.h"
#include "Swapchain.h"


class UniformBuffer : BufferBase
{
public:
	UniformBuffer(int bytesizeUniformBufferObject, Swapchain& swapchain,
												   GraphicsDevice& device);
	~UniformBuffer();

		// MEMBERS
private:
	vector<VkBuffer>		uniformBuffers;
	vector<VkDeviceMemory>	uniformBuffersMemory;

	uint32_t				numBuffers;	 // will == numSwapchainImages !

	VkDeviceSize			nbytesBufferObject;

		// METHODS
private:
	void create();
	void destroy();
public:
	void Update(int indexCurrentImage, void* pUBO, size_t nbytesUBO);
	void Recreate(int bytesizeUniformBufferObject, Swapchain& swapchain);

		// getters
	VkDescriptorBufferInfo getDescriptorBufferInfo() {
		return {
			.buffer	= uniformBuffers[0],
			.offset	= 0,
			.range	= nbytesBufferObject	// (or VK_WHOLE_SIZE)
		};
	}
};

#endif // UniformBuffer_h
