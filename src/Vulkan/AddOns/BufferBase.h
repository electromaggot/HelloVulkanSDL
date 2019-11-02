//
// BufferBase.h
//	Vulkan Add-ons
//
// Base class for general buffer operations amongst specific buffer implementations.
//	Specifically: creating a general buffer and finding suitable memory for it.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef BufferBase_h
#define BufferBase_h

#include "VulkanPlatform.h"
#include "GraphicsDevice.h"


class BufferBase
{
protected:
	BufferBase(GraphicsDevice& graphicsDevice)
		:	device(graphicsDevice.getLogical()),
			physicalDevice(graphicsDevice.getGPU())
	{ }

		// MEMBERS
	VkDevice&		  device;
	VkPhysicalDevice& physicalDevice;

		// METHODS

	void createGeneralBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
							 VkMemoryPropertyFlags properties,
							 VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {
			.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext	= nullptr,
			.flags	= 0,
			.size		 = size,
			.usage		 = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount	= 0,
			.pQueueFamilyIndices	= nullptr
		};

		call = vkCreateBuffer(device, &bufferInfo, nullALLOC, &buffer);
		if (call != VK_SUCCESS)
			Fatal("Create Buffer FAILURE" + ErrStr(call));

		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements(device, buffer, &memReqs);

		VkMemoryAllocateInfo allocInfo = {
			.sType	= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext	= nullptr,
			.allocationSize	 = memReqs.size,
			.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, properties)
		};

		call = vkAllocateMemory(device, &allocInfo, nullALLOC, &bufferMemory);	// (**)
		if (call != VK_SUCCESS)
			Fatal("Allocate Memory FAILURE" + ErrStr(call));

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags reqBits)
	{
		VkPhysicalDeviceMemoryProperties memProps;

		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

		for (uint32_t iProperty = 0; iProperty < memProps.memoryTypeCount; ++iProperty)
			if (typeFilter & (1 << iProperty)
				&& (memProps.memoryTypes[iProperty].propertyFlags & reqBits) == reqBits)
				return iProperty;

		return Fatal("Failed to find suitable memory type.");
	}
};

#endif // BufferBase_h


/* DEV NOTE
 (*) - Note that according to this:
 https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer#page_Conclusion
 calls to vkAllocateMemory should be limited.  Instead of making singular
 allocations, they should be combined/stacked and the .offset parameter used.
 Also refers to: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
*/
