//
// Vertex2DTextured.h
//	Vulkan Vertex-based Add-ons
//
// Encapsulate a Vertex with: 2D position and texturing
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef Vertex2DTextured_h
#define Vertex2DTextured_h

#include "VertexType.h"


struct Vertex2DTextured		// (Note: must not inherit, but be plain-old-data
{							//	type (no virtuals, standard-layout/trivial)
	vec2 position;			//	thus will have a simple default constructor
	vec2 texCoord;			//	and fixed, compiler-independent size/layout.)
};

struct VertexType2DTextured : VertexType
{
	const VkVertexInputAttributeDescription attributeDescriptions[2] = {
		{
			.location	= 0,	// These must match the above struct precisely!
			.binding	= 0,
			.format		= VK_FORMAT_R32G32_SFLOAT,
			.offset		= offsetof(Vertex2DTextured, position)
		}, {
			.location	= 1,
			.binding	= 0,
			.format		= VK_FORMAT_R32G32_SFLOAT,
			.offset		= offsetof(Vertex2DTextured, texCoord)
		}
	};

	const VkVertexInputBindingDescription bindingDescription = {
		.binding	= 0,
		.stride		= sizeof(Vertex2DTextured),
		.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX
	};

public:
	size_t	 byteSize()				  { return sizeof(Vertex2DTextured); }

	uint32_t nBindingDescriptions()	  { return 1; }
	uint32_t nAttributeDescriptions() { return N_ELEMENTS_IN_ARRAY(attributeDescriptions); }

	const VkVertexInputAttributeDescription* pAttributeDescriptions() {
		return attributeDescriptions;
	}
	const VkVertexInputBindingDescription* pBindingDescriptions() {
		return &bindingDescription;
	}

};

extern VertexType2DTextured		VertexDescriptor2DTextured;


#endif // Vertex2DTextured_h
