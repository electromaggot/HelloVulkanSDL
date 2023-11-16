#version 450
#extension GL_ARB_separate_shader_objects : enable

											// From Vertex2DColored, for each vertex:
layout(location = 0) in vec2 inPosition;		// 2D XY vertex coordinate
layout(location = 1) in vec3 inColor;			// RGB color

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}
