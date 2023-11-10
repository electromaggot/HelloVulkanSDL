//
// Quad2D.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded 2D shape for test, demo,
//	example, or any other purpose.
// Specifies an Index Buffer, so the Quad
//	is defined with 4 vertices instead of 6.
//
// Winding is counter-clockwise, which by our default
//	should make the fronts/normals face outward.
// Per Vulkan defaults, negative-Y is up.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#include "MeshObject.h"
#include "Vertex2D.h"


const Vertex2D QuadVertices[] = {

	{ -0.5f, -0.5f },
	{ -0.5f,  0.5f },
	{  0.5f,  0.5f },
	{  0.5f, -0.5f }
};

const IndexBufferIndexType QuadIndices[] = {

	0, 1, 2, 2, 3, 0
};

MeshObject Quad2D = {

	VertexDescriptor2D,
	(void*) QuadVertices,
	N_ELEMENTS_IN_ARRAY(QuadVertices),
	0,	// first vertex
	1,	// instance count
	0,	// first instance

	(void*) QuadIndices,
	N_ELEMENTS_IN_ARRAY(QuadIndices),
	0,	// first index
	0	// vertex offset
};



#if ASCII_ART
                -y
                  ^
         0        |        3
          o ,-----------. o
(-.5,-.5)   \     |      \  (.5,-.5)
          v  `.|  |  CCW  `
          |   ¯¯  |       |
 -x <-----|-------+-------|-----> +x
          |       |  __   |
          .  CCW  |  |.   ^
 (-.5,.5)  \      |    `.   (.5,.5)
          o `-----------´ o
         1        |        2
                  v
                   +y
#endif
