//
// GraphicsPipeline.h
//	Vulkan Setup
//
// Encapsulate the Pipeline.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at whim)
//
#ifndef GraphicsPipeline_h
#define GraphicsPipeline_h

#include "GraphicsDevice.h"
#include "ShaderModules.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "VertexType.h"
#include "Descriptors.h"


class GraphicsPipeline
{
public:
	GraphicsPipeline(ShaderModules& shaders, RenderPass& renderPass,
					 Swapchain& swapchain, GraphicsDevice& graphics,
					 VertexType* pVertex = nullptr, Descriptors* pDescriptors = nullptr);
	~GraphicsPipeline();

		// MEMBERS
private:
	VkPipelineLayout pipelineLayout;
	VkPipeline		 graphicsPipeline;

	VkDevice		 device;

		// METHODS
private:
	void create(ShaderModules& shaderModules, VertexType* pVertex,
				VkExtent2D swapchainExtent, VkRenderPass renderPass, Descriptors* pDescriptors);
	void destroy();
public:
	void Recreate(ShaderModules& shaderModules, RenderPass& renderPass,
				  Swapchain& swapchain, VertexType* pVertex = nullptr,
								  Descriptors* pDescriptors = nullptr);
		// getters
	VkPipeline&  getVkPipeline()			{ return graphicsPipeline;	}
	VkPipelineLayout&  getPipelineLayout()	{ return pipelineLayout;	}
};

#endif // GraphicsPipeline_h
