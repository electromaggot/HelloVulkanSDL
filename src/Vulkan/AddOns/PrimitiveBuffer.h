//
// PrimitiveBuffer.h
//	Vulkan Setup
//
// Abstraction for Vertex and Index Buffers, but potentially any bufferable
//	graphics/geometry element related to a primitive or triangle.  Includes
//	intermediate ("staging") buffer for Vulkan-command-queued transfer of
//	data from CPU/main RAM to, e.g., VRAM.
// Passed-in dependency VkCommandPool can either be the same one used for drawing
//	commands (easy) or a separate pool using VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
//	and short-lived buffers that may be optimized for memory allocations.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef PrimitiveBuffer_h
#define PrimitiveBuffer_h

#include "CommandBufferBase.h"
#include "DeviceQueues.h"
#include "VertexBasedObject.h"


class PrimitiveBuffer : CommandBufferBase
{
public:
	PrimitiveBuffer(VertexBasedObject& vertexObject, VkCommandPool& pool, GraphicsDevice& device);
	PrimitiveBuffer(IndexBufferIndexType* pIndices, uint32_t nIndices, VkCommandPool& pool, GraphicsDevice& device);
	PrimitiveBuffer(VkCommandPool& pool, GraphicsDevice& device);
	~PrimitiveBuffer();

		// MEMBERS
private:
	VkBuffer		  buffer;
	VkDeviceMemory	  bufferMemory;

		// METHODS
public:
	void	 CreateVertexBuffer(vector<VertexType> vertices);
	void	 CreateIndexBuffer(vector<IndexBufferIndexType> indices);
private:
	void	 createDeviceLocalBuffer(void* pSourceData, VkDeviceSize size, VkBufferUsageFlags usage,
									 VkBuffer& deviceBuffer, VkDeviceMemory& deviceMemory);
	void	 copyBufferViaVulkan(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		// getter
public:
	VkBuffer&	getVkBuffer()	{ return buffer; }
};

#endif // PrimitiveBuffer_h
