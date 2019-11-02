//
// UniformBufferObjects.h
//	Vulkan Add-ons
//
// Define any commonly-used UBOs.
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef UniformBufferObjects_h
#define UniformBufferObjects_h

#include "VertexType.h"


enum DestinationStage {
	DESTINATION_VERTEX_STAGE	= VK_SHADER_STAGE_VERTEX_BIT,
	DESTINATION_FRAGMENT_STAGE	= VK_SHADER_STAGE_FRAGMENT_BIT,
	DESTINATION_UNKNOWN			= -1
};


// Model-/View-/Projection-matrix UBOs are usually destined for
//	the Vertex Stage, so that will be their default below.
//
struct UBO_MVP {
	alignas(16)		mat4 model;
	alignas(16)		mat4 view;
	alignas(16)		mat4 proj;
};

// This UBO passes real-time state used by a Fragement Shader
//	renderer, e.g. a Ray-based algorithm, so it is intended for
//	the Fragment Stage, which is its default value below.
//
struct UBO_rtm {
	alignas(16)		vec4  resolution;
	//alignas(12)	vec3  resolution;	// <-- DID NOT WORK! See NOTE 2 below.
	alignas(4)		float time;
	alignas(16)		vec4  mouse;
};


struct UBO {
	int					byteSize;
	void*				pUBO;
	DestinationStage	destinationStage;

	UBO(UBO_MVP& mvp, DestinationStage dstage = DESTINATION_VERTEX_STAGE)
		:	byteSize(sizeof(UBO_MVP)), pUBO(&mvp), destinationStage(dstage)	  { }

	UBO(UBO_rtm& rtm, DestinationStage dstage = DESTINATION_FRAGMENT_STAGE)
		:	byteSize(sizeof(UBO_rtm)), pUBO(&rtm), destinationStage(dstage)	  { }

	UBO() : byteSize(0), pUBO(nullptr), destinationStage(DESTINATION_UNKNOWN) { }

	VkShaderStageFlags	getShaderStageFlags() {
		assert(destinationStage != DESTINATION_UNKNOWN);
		return (VkShaderStageFlags) destinationStage;
	}
};


#endif // UniformBufferObjects_h


/* DEV NOTE
	Alignment issues CPU-side vs. GPU-side:
	https://vulkan-tutorial.com/Uniform_buffers/Descriptor_pool_and_sets#page_Alignment-requirements
*/

/* NOTE 2
	Using "vec3" seemed to lead to a variable-offset-mismatch on the GPU side!  Specifically,
	graphically at runtime, mouseX appeared to increment in time while mouseY remained 0.
	It seemed that resolution was 16 bytes here but 12 there, so the last float of the
	now-vec4 (zero) went into 'time' on the GPU, and 'time' here went into mouseX there.
	Attempting 'alignas(12)' above as corrective measure is illegal: not a power of two.
*/
