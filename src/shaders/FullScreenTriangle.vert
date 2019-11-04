#version 450 

// One triangle, appropriately UV-mapped, provides a full-screen rendering surface.
//	Cleverly avoids the additional triangle required by a quad, accomplishing the same thing.

// All credit to Sascha Willems (https://www.saschawillems.de/blog/2016/08/13/vulkan-tutorial-on-rendering-a-fullscreen-quad-without-buffers/)
//	Note however that Tadd did rework slightly to wind counter-clockwise and backface-cull.


layout (location = 0) out vec2 outUV;

void main() 
{
	outUV = vec2(gl_VertexIndex & 2, (gl_VertexIndex << 1) & 2);
	gl_Position = vec4(outUV * 2.0f + -1.0f, 0.0f, 1.0f);
}



/* DEV NOTE
	This same trick can be used for single-triangle billboards or particles -- halving your
	number of renderables (!) -- given correct clamping, e.g.:
	 - VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE - with a contiguously transparent outside edge.
	or
	 - VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER - while setting  VkBorderColor  to:
			VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK  or  VK_BORDER_COLOR_INT_TRANSPARENT_BLACK
*/

/* DEV NOTE 2 (a study on reversing the winding)

ONE TRIANGLE Vertex Shader
working out how it works

	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(outUV * 2.0f + -1.0f, 0.0f, 1.0f);

VertexIndex		U	V		X	Y	Z	W
   ---		   --- ---	   --- --- --- ---
	0			0	0	   -1  -1	0	1
	1			2	0		3	0	0	1
	2			0	2		0	3	0	1

    -1        0        1        2        3
     |        |        |        |        |
   #0                                     #1 <-- CW
-1 - o--------+--------+--------+--------o
     |(-1,-1)          |             _-'  (3,0)
     |        '        U        ' _-'    |
 0 - +  -  -  +(0,0)   =  -  - _-'       U
     |                 1    _-'          =
     |        '        | _-'             2
 1 - +---V=1-----------+'
     |             _-' (1,1)
     |        ' _-'
 2 - +  -  - _-'
     |    _-'
     | _-'
 3 - o'  -- V=2
   #2 (0,3)


To make COUNTER-CLOCKWISE, would need:

VertexIndex		U	V		X	Y	Z	W
   ---		   --- ---	   --- --- --- ---
	0			0	0	   -1  -1	0	1
	1			0	2		0	3	0	1
	2			2	0		3	0	0	1

I think this means just reversing the U,V components
(which reverses the X,Y components)

    outUV = vec2(gl_VertexIndex & 2, (gl_VertexIndex << 1) & 2);
    gl_Position = vec4(outUV * 2.0f + -1.0f, 0.0f, 1.0f);

This is of course now compatible with these PIPELINE settings:

	rasterizationState.cullMode = VK_CULL_MODE_REAR_BIT;
	rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

Culling of backfaces is more standard.
*/

