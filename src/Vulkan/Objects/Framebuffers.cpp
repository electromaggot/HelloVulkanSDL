//
// Framebuffers.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "Framebuffers.h"
#include "VulkanSingleton.h"


Framebuffers::Framebuffers(Swapchain& swapchain, RenderPass& renderPass, GraphicsDevice& graphics)
	:	device(graphics.getLogical())
{
	create(swapchain.getImageViews(), swapchain.getExtent(), renderPass.getVkRenderPass());
}

Framebuffers::~Framebuffers()  { destroy(); }

void Framebuffers::destroy()
{
	for (auto& framebuffer : framebuffers)
		vkDestroyFramebuffer(device, framebuffer, nullALLOC);
}


void Framebuffers::create(vector<VkImageView>& imageViews, VkExtent2D& extent,
														VkRenderPass renderPass)
{
	size_t nImageViews = imageViews.size();

	framebuffers.resize(nImageViews);

	for (size_t iBufferView = 0; iBufferView < nImageViews; ++iBufferView)
	{
		VkImageView attachments[] = {

			imageViews[iBufferView]
		};

		VkFramebufferCreateInfo framebufferInfo = {
			.sType	= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext	= nullptr,
			.flags	= 0,
			.renderPass		 = renderPass,
			.attachmentCount = 1,
			.pAttachments	 = attachments,
			.width			 = extent.width,
			.height			 = extent.height,
			.layers			 = AppConstants.SupportStereo3D ? (uint32_t) 2 : 1
		};

		call = vkCreateFramebuffer(device, &framebufferInfo, nullALLOC, &framebuffers[iBufferView]);

		if (call != VK_SUCCESS)
			Fatal("Create Framebuffer FAILURE" + ErrStr(call));
	}
}

void Framebuffers::Recreate(Swapchain& swapchain, RenderPass& renderPass)
{
	destroy();
	create(swapchain.getImageViews(), swapchain.getExtent(), renderPass.getVkRenderPass());
}
