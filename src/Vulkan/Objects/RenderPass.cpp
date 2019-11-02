//
// RenderPass.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "RenderPass.h"


RenderPass::RenderPass(GraphicsDevice& graphicsDevice)
	:	device(graphicsDevice.getLogical())
{
	create(graphicsDevice.getProfile().selectedSurfaceFormat.format);
}

RenderPass::~RenderPass()
{
	vkDestroyRenderPass(device, renderPass, nullALLOC);
}


void RenderPass::create(VkFormat imageFormat)
{
	VkAttachmentDescription colorAttachment = {
		.flags			= 0,
		.format			= imageFormat,
		.samples		= VK_SAMPLE_COUNT_1_BIT,
		.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp		= VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	VkAttachmentReference colorAttachmentRef = {
		.attachment	= 0,
		.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass = {
		.flags					 = 0,
		.pipelineBindPoint		 = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.inputAttachmentCount	 = 0,
		.pInputAttachments		 = nullptr,
		.colorAttachmentCount	 = 1,
		.pColorAttachments		 = &colorAttachmentRef,
		/*.pResolveAttachments	 = nullptr,
		.pDepthStencilAttachment = nullptr,
		.preserveAttachmentCount = 0,
		.pPreserveAttachments	 = nullptr*/
	};

	VkSubpassDependency dependency = {
		.srcSubpass		 = VK_SUBPASS_EXTERNAL,
		.dstSubpass		 = 0,
		.srcStageMask	 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask	 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask	 = 0,
		.dstAccessMask	 = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
						 | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.dependencyFlags = 0
	};

	VkRenderPassCreateInfo renderPassInfo = {
		.sType	= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0,
		.attachmentCount = 1,
		.pAttachments	 = &colorAttachment,
		.subpassCount	 = 1,
		.pSubpasses		 = &subpass,
		.dependencyCount = 1,
		.pDependencies	 = &dependency
	};

	call = vkCreateRenderPass(device, &renderPassInfo, nullALLOC, &renderPass);

	if (call != VK_SUCCESS)
		Fatal("Create Render Pass FAILURE" + ErrStr(call));
}
