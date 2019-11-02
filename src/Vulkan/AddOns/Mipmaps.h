//
// Mipmaps.h
//	Vulkan Add-ons
//
// Encapsulate mipmapping, including explicit creation.
//
// Created 7/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "CommandBufferBase.h"
#include <cmath>


class Mipmaps : CommandBufferBase
{
public:
	Mipmaps(VkCommandPool& pool, GraphicsDevice& device)
		:	CommandBufferBase(pool, device),
			numLevels(1)
	{ }

		// MEMBER
private:
	uint32_t	numLevels;

		// METHODS
public:
	uint32_t CalculateNumberOfLevels(int32_t textureWidth, int32_t textureHeight)
	{
		numLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(textureWidth, textureHeight)))) + 1;
		return numLevels;
	}

	uint32_t NumLevels()	{ return numLevels; }


	void Generate(VkImage image, VkFormat imageFormat, int32_t textureWide, int32_t textureHigh)
	{
		if (numLevels <= 1)
			Log(WARN, "Using MIPMAPS, but Number of Levels is %d (should be > 1) or was not Calculate()d in VkImageCreateInfo.", numLevels);

		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
			Fatal("Texture image format does not support linear blitting.");

		VkCommandBuffer commandBuffer = beginSingleSubmitCommands();

		VkImageMemoryBarrier barrier = {
			.sType	= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.pNext	= nullptr,
			.srcAccessMask	= VK_ACCESS_TRANSFER_WRITE_BIT,
			.dstAccessMask	= VK_ACCESS_TRANSFER_READ_BIT,
			.oldLayout			= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.newLayout			= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			.srcQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED,
			.image			= image,
			.subresourceRange = {
				.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel	= 0,
				.levelCount		= 1,
				.baseArrayLayer	= 0,
				.layerCount		= 1
			}
		};

		int32_t mipWide  = textureWide;
		int32_t mipHigh  = textureHigh;

		for (uint32_t iLevel = 1; iLevel < numLevels; ++iLevel) {

			vkCmdPipelineBarrier(commandBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			VkImageBlit blit = {
				.srcSubresource = {
					.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel		= iLevel - 1,
					.baseArrayLayer	= 0,
					.layerCount		= 1
				},
				.srcOffsets =	{	{ 0, 0, 0 },						// [0]
									{ mipWide, mipHigh, 1 }				// [1]
								},
				.dstSubresource = {
					.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel		= iLevel,
					.baseArrayLayer	= 0,
					.layerCount		= 1
				},
				.dstOffsets =	{	{ 0, 0, 0 },						// [0]
									{ mipWide > 1 ? mipWide / 2 : 1,	 //
									  mipHigh > 1 ? mipHigh / 2 : 1,	// [1]
									  1 }							   //
								}
			};

			vkCmdBlitImage(commandBuffer,
						   image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						   image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						   1, &blit,
						   VK_FILTER_LINEAR);

			barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask	= VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask	= VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			if (mipWide > 1)  mipWide /= 2;
			if (mipHigh > 1)  mipHigh /= 2;

			barrier.subresourceRange.baseMipLevel = iLevel;
			barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask	= VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask	= VK_ACCESS_TRANSFER_READ_BIT;
		}

		barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.dstAccessMask	  = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
							 0, nullptr,
							 0, nullptr,
							 1, &barrier);

		endAndSubmitCommands(commandBuffer);
	}
};
