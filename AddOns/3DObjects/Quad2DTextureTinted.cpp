//
// Quad2DTextureTinted.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded 2D shape for test, demo,
//	example, or any other purpose.
// Applies a texture but "tints" it via
//	colored vertices: red, white, blue, green.
// Specifies an Index Buffer, so the Quad
//	may define 4 vertices instead of 6.
//
// Winding is counter-clockwise, which by our default
//	should make the fronts/normals face outward.
// Per Vulkan defaults, negative-Y is up.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#include "MeshObject.h"
#include "Vertex2DTextureTinted.h"


const Vertex2DTextureTinted QuadVertices[] = {

	{ {	-0.5f, -0.5f },	{ 1.0f, 0.0f, 0.0f }, {	0.0f, 0.0f } },
	{ {	-0.5f,  0.5f },	{ 1.0f, 1.0f, 1.0f }, {	0.0f, 1.0f } },
	{ {	 0.5f,  0.5f },	{ 0.0f, 0.0f, 1.0f }, {	1.0f, 1.0f } },
	{ {	 0.5f, -0.5f },	{ 0.0f, 1.0f, 0.0f }, {	1.0f, 0.0f } }
};

const IndexBufferIndexType QuadIndices[] = {

	0, 1, 2, 2, 3, 0
};

MeshObject Quad2DTextureTinted = {

	VertexDescriptor2DTextureTinted,
	(void*) QuadVertices,
	N_ELEMENTS_IN_ARRAY(QuadVertices),

	(void*) QuadIndices,
	N_ELEMENTS_IN_ARRAY(QuadIndices)
};
