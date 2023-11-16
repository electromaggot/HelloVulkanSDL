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
#include "Vertex2DTypes.h"


const vec4 white { 1.0f, 1.0f, 1.0f, 1.0f };
const vec4 blue	 { 0.0f, 0.0f, 1.0f, 1.0f };
const vec4 green { 0.0f, 1.0f, 0.0f, 1.0f };


VertexType2DColor	VertexDescriptor2DColor;	// example of non-templated "long hand" description,
												//	see struct at bottom of: VertexDescription.h

const Vertex2DColor TriangleVertices[] = {

	{ {	 0.0f, -0.5f },	 white	},
	{ {	-0.5f,  0.5f },	 blue	},
	{ {	 0.5f,  0.5f },	 green	}
};


MeshObject Triangle2DColored = {

	VertexDescriptor2DColor,
	(void*) TriangleVertices,
	N_ELEMENTS_IN_ARRAY(TriangleVertices)
};



#if ASCII_ART_EXPLAINER

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
