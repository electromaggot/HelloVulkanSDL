#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location = 0) out vec4 outColor;


void main()
{
	vec3 scaler = vec3(40.0, 30.0, 1.0);

	// Normalized pixel coordinates (from 0 to 1)
	vec2 uv = gl_FragCoord.xy / scaler.xy;

	// Position-varying pixel color
	vec3 col = 0.5 + 0.5 * cos(uv.xyx + vec3(0,2,4));

	// Output to screen
	outColor = vec4(col, 1.0);
}

