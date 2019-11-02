//
// VertexNull.h
//	Vulkan Vertex-based Add-ons
//
// A "DON'T CARE" empty Vertex.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef VertexNull_h
#define VertexNull_h

#include "VertexType.h"


struct VertexTypeNull : VertexType
{
public:
	size_t	 byteSize()				  { return 0; }

	uint32_t nBindingDescriptions()	  { return 0; }
	uint32_t nAttributeDescriptions() { return 0; }

	const VkVertexInputAttributeDescription* pAttributeDescriptions() {
		return nullptr;
	}
	const VkVertexInputBindingDescription* pBindingDescriptions() {
		return nullptr;
	}

};

extern VertexTypeNull	VertexNull;


#endif // VertexNull_h
