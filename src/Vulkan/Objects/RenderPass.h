//
// RenderPass.h
//	Vulkan Setup
//
// Encapsulate the process to initialize/create VkRenderPass.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef RenderPass_h
#define RenderPass_h

#include "GraphicsDevice.h"
#include "Swapchain.h"


class RenderPass
{
		// XSTRUCT
public:
	RenderPass(GraphicsDevice& graphicsDevice);
	~RenderPass();

		// MEMBERS
private:
	VkRenderPass renderPass;

	VkDevice&	 device;		// Save to destruct as constructed

		// METHODS
private:
	void	create(VkFormat imageFormat);

		// getters
public:
	VkRenderPass& getVkRenderPass()		{ return renderPass; }
};

#endif // RenderPass_h
