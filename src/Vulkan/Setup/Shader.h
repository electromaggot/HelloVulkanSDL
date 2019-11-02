//
// Shader.h
//	Vulkan Setup
//
// Shader abstraction: generically specify shaders.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef Shader_h
#define Shader_h


enum ShaderType {

	VERTEX_SHADER		= VK_SHADER_STAGE_VERTEX_BIT,
	FRAGMENT_SHADER 	= VK_SHADER_STAGE_FRAGMENT_BIT,

	GEOMETRY_SHADER		= VK_SHADER_STAGE_GEOMETRY_BIT,
	TESSELATION_SHADER	= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,

	COMPUTE_SHADER		= VK_SHADER_STAGE_COMPUTE_BIT
};


struct Shader
{
	ShaderType	fileType;
	StrPtr		nameShaderFile;
	StrPtr		nameEntrypointFunction;
};

typedef vector<Shader> Shaders;
/*struct Shaders
{
	int		nShaders;
	Shader	array[];
};*/


#endif // Shader_h
