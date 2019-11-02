//
// Vertex2DColored.h
//	Vulkan Vertex-based Add-ons
//
// Encapsulate a Vertex with: 2D position, per-vertex color attribute
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef Vertex2DColored_h
#define Vertex2DColored_h

#include "VertexType.h"


struct Vertex2DColored		// (Note: must not inherit, but be plain-old-data
{							//	type (no virtuals, standard-layout/trivial)
	vec2 position;			//	thus will have a simple default constructor
	vec3 color;				//	and fixed, compiler-independent size/layout.)
};

struct VertexType2DColored : VertexType
{
	const VkVertexInputAttributeDescription attributeDescriptions[2] = {
		{
			.location	= 0,	// These must match the above struct precisely!
			.binding	= 0,
			.format		= VK_FORMAT_R32G32_SFLOAT,
			.offset		= offsetof(Vertex2DColored, position)
		}, {
			.location	= 1,
			.binding	= 0,
			.format		= VK_FORMAT_R32G32B32_SFLOAT,
			.offset		= offsetof(Vertex2DColored, color)
		}
	};

	const VkVertexInputBindingDescription bindingDescription = {
		.binding	= 0,
		.stride		= sizeof(Vertex2DColored),
		.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX
	};

public:
	size_t	 byteSize()				  { return sizeof(Vertex2DColored); }

	uint32_t nBindingDescriptions()	  { return 1; }
	uint32_t nAttributeDescriptions() { return N_ELEMENTS_IN_ARRAY(attributeDescriptions); }

	const VkVertexInputAttributeDescription* pAttributeDescriptions() {
		return attributeDescriptions;
	}
	const VkVertexInputBindingDescription* pBindingDescriptions() {
		return &bindingDescription;
	}

};

extern VertexType2DColored	VertexDescriptor2DColored;


#endif // Vertex2DColored_h
