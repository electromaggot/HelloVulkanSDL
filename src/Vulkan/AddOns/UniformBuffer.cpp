//
// UniformBuffer.cpp
//	Vulkan Add-ons
//
// See header description.
//
// Note that herein:
//	numBuffers = numSwapchainImages;
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "UniformBuffer.h"


UniformBuffer::UniformBuffer(int bytesizeUniformBufferObject, Swapchain& swapchain,
							 								  GraphicsDevice& device)
	:	BufferBase(device),
		numBuffers(static_cast<uint32_t>(swapchain.getImageViews().size())),
		nbytesBufferObject(bytesizeUniformBufferObject)
{
	create();
}

UniformBuffer::~UniformBuffer()
{
	destroy();
}

void UniformBuffer::destroy()
{
	for (int iBuffer = 0; iBuffer < numBuffers; ++iBuffer) {
		vkDestroyBuffer(device, uniformBuffers[iBuffer], nullALLOC);
		vkFreeMemory(device, uniformBuffersMemory[iBuffer], nullALLOC);
	}
}


void UniformBuffer::create()
{
	uniformBuffers.resize(numBuffers);
	uniformBuffersMemory.resize(numBuffers);

	for (int iBuffer = 0; iBuffer < numBuffers; ++iBuffer)
		createGeneralBuffer(nbytesBufferObject, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
							uniformBuffers[iBuffer], uniformBuffersMemory[iBuffer]);
}


void UniformBuffer::Update(int indexCurrentImage, void* pUBO, size_t nbytesUBO)
{
	void* data;
	vkMapMemory(device, uniformBuffersMemory[indexCurrentImage], 0, nbytesUBO, 0, &data);
	memcpy(data, pUBO, nbytesUBO);
	vkUnmapMemory(device, uniformBuffersMemory[indexCurrentImage]);
}


void UniformBuffer::Recreate(int bytesizeUniformBufferObject, Swapchain& swapchain)
{
	if (bytesizeUniformBufferObject >= 0)	// otherwise keep same sized buffer as before
		nbytesBufferObject = bytesizeUniformBufferObject;
	numBuffers = static_cast<uint32_t>(swapchain.getImageViews().size());

	destroy();
	create();
}
