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
