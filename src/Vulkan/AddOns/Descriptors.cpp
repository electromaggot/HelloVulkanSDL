//
// Descriptors.cpp
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
#include "Descriptors.h"


Descriptors::Descriptors(vector<Described>& describeds, Swapchain& swapchain, GraphicsDevice& device)
	:	BufferBase(device),
		describers(describeds),
		numBuffers(static_cast<uint32_t>(swapchain.getImageViews().size()))
{
	createDescriptorSetLayout();
	create();
}

Descriptors::~Descriptors()
{
	destroy();
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

void Descriptors::create()
{
	createDescriptorPool();
	createDescriptorSets();
}

void Descriptors::destroy()
{
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);	// (note: destroys descriptorSets too)
}


void Descriptors::createDescriptorSetLayout()
{
	uint32_t numDescribers = (uint32_t) describers.size();
	VkDescriptorSetLayoutBinding	layoutBindings[numDescribers];

	for (uint32_t iBind = 0; iBind < numDescribers; ++iBind)
		layoutBindings[iBind] = {
			.binding  = iBind,
			.descriptorType	 = describers[iBind].getDescriptorType(),
			.descriptorCount = 1,
			.stageFlags			= describers[iBind].getShaderStageFlags(),
			.pImmutableSamplers = nullptr
		};

	VkDescriptorSetLayoutCreateInfo layoutInfo = {
		.sType	= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0,
		.bindingCount = numDescribers,
		.pBindings	  = layoutBindings
	};

	call = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);

	if (call != VK_SUCCESS)
		Fatal("Create Descriptor Set Layout FAILURE" + ErrStr(call));
}


void Descriptors::createDescriptorPool()
{
	uint32_t numDescribers = (uint32_t) describers.size();
	VkDescriptorPoolSize	poolSizes[numDescribers];

	for (int iPool = 0; iPool < numDescribers; ++iPool)
		poolSizes[iPool] = {
			.type	= describers[iPool].getDescriptorType(),
			.descriptorCount  = numBuffers
		};

	VkDescriptorPoolCreateInfo poolInfo = {
		.sType	= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0,
		.maxSets	= numBuffers,
		.poolSizeCount	= numDescribers,
		.pPoolSizes		= poolSizes
	};

	call = vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);
	if (call != VK_SUCCESS)
		Fatal("Create Descriptor Pool" + ErrStr(call));
}

void Descriptors::createDescriptorSets()
{
	uint32_t numDescribers = (uint32_t) describers.size();
	vector<VkDescriptorSetLayout> layouts(numBuffers, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {
		.sType	= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext	= nullptr,
		.descriptorPool		= descriptorPool,
		.descriptorSetCount	= numBuffers,
		.pSetLayouts  = layouts.data()
	};
	descriptorSets.resize(numBuffers);

	call = vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data());
	if (call != VK_SUCCESS)
		Fatal("Allocate Descriptor Sets FAILURE" + ErrStr(call));

	for (int iBuffer = 0; iBuffer < numBuffers; ++iBuffer)
	{
		VkWriteDescriptorSet descriptorWrite[numDescribers];

		for (uint32_t iBind = 0; iBind < numDescribers; ++iBind) {

			VkDescriptorBufferInfo* pBufferInfo	= nullptr;
			VkDescriptorImageInfo*	pImageInfo	= nullptr;

			switch (describers[iBind].type) {
				case BUFFER:
					pBufferInfo = &describers[iBind].bufferInfo;
					break;
				case TEXTURE:
					pImageInfo = &describers[iBind].imageInfo;
					break;
			}

			descriptorWrite[iBind] = {
				.sType	= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext	= nullptr,
				.dstSet			 = descriptorSets[iBuffer],
				.dstBinding		 = iBind,
				.dstArrayElement = 0,
				.descriptorCount	= 1,
				.descriptorType		= describers[iBind].getDescriptorType(),
				.pImageInfo	 		= pImageInfo,
				.pBufferInfo		= pBufferInfo,
				.pTexelBufferView	= nullptr
			};

		}
		vkUpdateDescriptorSets(device, numDescribers, descriptorWrite, 0, nullptr);
	}								// ^write descriptor^			   ^copy descriptor^
}


void Descriptors::Recreate(vector<Described> descriptions, Swapchain& swapchain)
{
	destroy();

	describers = descriptions;	// copy via assignment operator
	numBuffers = static_cast<uint32_t>(swapchain.getImageViews().size());

	create();
}


/* DEV NOTE
 Pondering VkDescriptorTypes...
 Texture-related
	VK_DESCRIPTOR_TYPE_SAMPLER = 0
	VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER = 1
	VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE = 2
	VK_DESCRIPTOR_TYPE_STORAGE_IMAGE = 3
 Mixed? (perhaps more on the "uniform" side)
	VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER = 4
	VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER = 5
 Uniform-Buffer-related
	VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7
	VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC = 8
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC = 9
 Don't know
	VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT = 10
	VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT = 1000138000
	VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NVX = 1000165000
*/
