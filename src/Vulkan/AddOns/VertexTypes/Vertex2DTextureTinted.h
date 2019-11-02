//
// Vertex2DTextureTinted.h
//	Vulkan Vertex-based Add-ons
//
// Encapsulate a Vertex with: 2D position, per-vertex color, and texturing
//	Vertex colors blend to "tint" the applied texture.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef Vertex2DTextureTinted_h
#define Vertex2DTextureTinted_h

#include "VertexType.h"


struct Vertex2DTextureTinted	// (Note: must not inherit, but be plain-old-data
{								//	type (no virtuals, standard-layout/trivial)
	vec2 position;				//	thus will have a simple default constructor
	vec3 color;					//	and fixed, compiler-independent size/layout.)
	vec2 texCoord;
};

struct VertexType2DTextureTinted : VertexType
{
	const VkVertexInputAttributeDescription attributeDescriptions[3] = {
		{
			.location	= 0,	// These must match the above struct precisely!
			.binding	= 0,
			.format		= VK_FORMAT_R32G32_SFLOAT,
			.offset		= offsetof(Vertex2DTextureTinted, position)
		}, {
			.location	= 1,
			.binding	= 0,
			.format		= VK_FORMAT_R32G32B32_SFLOAT,
			.offset		= offsetof(Vertex2DTextureTinted, color)
		}, {
			.location	= 2,
			.binding	= 0,
			.format		= VK_FORMAT_R32G32_SFLOAT,
			.offset		= offsetof(Vertex2DTextureTinted, texCoord)
		}
	};

	const VkVertexInputBindingDescription bindingDescription = {
		.binding	= 0,
		.stride		= sizeof(Vertex2DTextureTinted),
		.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX
	};

public:
	size_t	 byteSize()				  { return sizeof(Vertex2DTextureTinted); }

	uint32_t nBindingDescriptions()	  { return 1; }
	uint32_t nAttributeDescriptions() { return N_ELEMENTS_IN_ARRAY(attributeDescriptions); }

	const VkVertexInputAttributeDescription* pAttributeDescriptions() {
		return attributeDescriptions;
	}
	const VkVertexInputBindingDescription* pBindingDescriptions() {
		return &bindingDescription;
	}

};

extern VertexType2DTextureTinted	VertexDescriptor2DTextureTinted;


#endif // Vertex2DTextureTinted_h
