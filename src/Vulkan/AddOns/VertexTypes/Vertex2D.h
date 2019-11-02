//
// Vertex2D.h
//	Vulkan Vertex-based Add-ons
//
// Encapsulate a Vertex with 2D position.
//	(No Z coordinate, or Z assumed zero)
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef Vertex2D_h
#define Vertex2D_h

#include "VertexType.h"


typedef	vec2	Vertex2D;


struct VertexType2D : VertexType
{
	const VkVertexInputAttributeDescription attributeDescription = {
		.location	= 0,
		.binding	= 0,
		.format		= VK_FORMAT_R32G32_SFLOAT,	// i.e. 2D
		.offset		= 0
	};

	const VkVertexInputBindingDescription bindingDescription = {
		.binding	= 0,
		.stride		= sizeof(Vertex2D),
		.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX
	};

public:
	size_t	 byteSize()				  { return sizeof(Vertex2D); }

	uint32_t nBindingDescriptions()	  { return 1; }
	uint32_t nAttributeDescriptions() { return 1; }

	const VkVertexInputAttributeDescription* pAttributeDescriptions() {
		return &attributeDescription;
	}
	const VkVertexInputBindingDescription* pBindingDescriptions() {
		return &bindingDescription;
	}

};

extern VertexType2D	 VertexDescriptor2D;


#endif // Vertex2D_h
