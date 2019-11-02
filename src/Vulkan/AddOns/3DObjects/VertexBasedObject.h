//
// VertexBasedObject.h
//	Vulkan Vertex-based Add-on
//
// Represent a Vertex Array by an abstract Descriptor of vertex type,
//	a pointer to all the vertices, and the number of them.
// To keep the vertex array's initialization as simple as possible, while
//	its individual vertices are typed, their structure must be trivial/
//	plain-old-data, and especially, not inherited, like from an abstraction.
//	So unfortunately, the so-called "abstract type" has to be a void pointer.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef VertexObject_h
#define VertexObject_h

#include "VulkanPlatform.h"

#include "VertexType.h"

class PrimitiveBuffer;


struct VertexBasedObject
{
	VertexType&	 vertexType;
	void*		 vertices		= nullptr;
	uint32_t	 vertexCount	= 0;
	uint32_t	 firstVertex	= 0;

	VkDeviceSize vertexBufferSize() {
		return vertexCount * vertexType.byteSize();
	}

	uint32_t	 instanceCount	= 1;		// (shared between above Vertex
	uint32_t	 firstInstance	= 0;		//	Buffer and below Index Buffer)

	// (fyi, if you're not using indexes, you don't necessarily have to
	//	include any or all of the following in your pre-initializer code)

	void*		 indices		= nullptr;
	uint32_t	 indexCount		= 0;
	uint32_t	 firstIndex		= 0;
	int32_t		 vertexOffset	= 0;

	VkDeviceSize indexBufferSize() {
		return indexCount * sizeof(IndexBufferIndexType);
	}
};

#endif // VertexObject_h
