//
// VertexType.h
//	Vulkan Core Class
//
// Abstract Vertex Descriptor, for:
//	- deriving from, to define multivariate vertices
//	- passing around as a Liskov-substitutable
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#ifndef VertexType_h
#define VertexType_h

#if NO_GLM		// (if your needs are ultra simple...)
	typedef float vec2[2];
	typedef float vec3[3];	// Note that these floats match SFLOAT in VK_FORMAT.
	typedef float mat4[4][4];
#else
	// (Make sure glm's base directory is in your project's header paths!)

	#define GLM_FORCE_RADIANS
	#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>

	using glm::vec2; using glm::vec3; using glm::vec4; using glm::mat4;
	using glm::radians;
#endif


typedef uint16_t	IndexBufferIndexType;
	//  ^^^^^^^^-----(these two must jive!)----vvvvvvvvvvvvvvvvvvvv
const VkIndexType	IndexBufferIndexTypeEnum = VK_INDEX_TYPE_UINT16;


struct VertexType	// abstraction
{
public:
	virtual size_t	 byteSize() = 0;

	virtual uint32_t nBindingDescriptions()	  = 0;
	virtual uint32_t nAttributeDescriptions() = 0;

	virtual const VkVertexInputBindingDescription*	 pBindingDescriptions()	  = 0;
	virtual const VkVertexInputAttributeDescription* pAttributeDescriptions() = 0;
};

#endif // VertexType_h
