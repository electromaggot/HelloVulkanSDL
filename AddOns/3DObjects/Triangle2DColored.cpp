//
// Triangle2DColored.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded 2D shape for test, demo,
//	example, or any other purpose.
// Uses colored vertices: white, blue, green.
// Winding is counter-clockwise, which by our default
//	should make the front/normal face outward.
// Per Vulkan defaults, negative-Y is up.
//
// Created 6/14/19 by Tadd Jensen
//	© 2112 (uncopyrighted; use at will)
//
#include "MeshObject.h"
#include "Vertex2DColored.h"


const Vertex2DColored TriangleVertices[] = {

	{ {	 0.0f, -0.5f },	{ 1.0f, 1.0f, 1.0f } },
	{ {	-0.5f,  0.5f },	{ 0.0f, 0.0f, 1.0f } },
	{ {	 0.5f,  0.5f },	{ 0.0f, 1.0f, 0.0f } }
};

MeshObject Triangle2DColored = {

	VertexDescriptor2DColored,
	(void*) TriangleVertices,
	N_ELEMENTS_IN_ARRAY(TriangleVertices),
	0,	// first vertex
	1,	// instance count
	0	// first instance
};



#if ASCII_ART
               -y
                 ^
                 | (0, -.5)
               0 o
                 | .__
              L  | |\
             /   |   \
 -x <-------/----+----\-------> +x
           /     |     \
          /     CCW     \
(-.5,.5)  \      |      /  (.5,.5)
        o  `-----------´  o
       1         |         2
                 v
                  +y
#endif
