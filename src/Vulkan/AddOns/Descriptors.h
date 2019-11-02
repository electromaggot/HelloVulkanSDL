//
// Descriptors.h
//	Vulkan Add-ons
//
// "DESCRIBE" to Vulkan add-ons like a Uniform Buffer Object or
//	a Texture Image object.  Descriptors include Pool, Sets, Layout.
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef Descriptors_h
#define Descriptors_h

#include "BufferBase.h"
#include "Swapchain.h"


enum DescriptorGeneralType {
	BUFFER	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	TEXTURE = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};


struct Described {
	DescriptorGeneralType	type;
	union {
		VkDescriptorBufferInfo bufferInfo;
		VkDescriptorImageInfo  imageInfo;
	};
	VkShaderStageFlags		stage;

	Described(VkDescriptorBufferInfo bufinf, VkShaderStageFlags flags)
		:	type(BUFFER), bufferInfo(bufinf), stage(flags)			{ }
	Described(VkDescriptorImageInfo imginf, VkShaderStageFlags flags)
		:	type(TEXTURE), imageInfo(imginf), stage(flags)			{ }
	VkDescriptorType	getDescriptorType()		{ return (VkDescriptorType) type;	}
	VkShaderStageFlags	getShaderStageFlags()	{ return stage;						}
};



class Descriptors : BufferBase
{
public:
	Descriptors(vector<Described>& describeds, Swapchain& swapchain, GraphicsDevice& device);
	~Descriptors();

		// MEMBERS
private:
	vector<Described>		describers;

	uint32_t				numBuffers;	 // will == numSwapchainImages !

	VkDescriptorPool		descriptorPool;
	vector<VkDescriptorSet>	descriptorSets;

	VkDescriptorSetLayout	descriptorSetLayout;

		// METHODS
private:
	void create();
	void destroy();
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();
public:
	void Recreate(vector<Described> descriptions, Swapchain& swapchain);

		// getters
	VkDescriptorSetLayout*		getLayout()			 { return &descriptorSetLayout;	}
	vector<VkDescriptorSet>&	getDescriptorSets()	 { return descriptorSets;		}
};

#endif // Descriptors_h
