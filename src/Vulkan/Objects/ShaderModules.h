//
// ShaderModules.h
//	Vulkan Setup
//
// Encapsulate all that goes into a VkShaderModule,
//	and multiple ones at that.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ShaderModules_h
#define ShaderModules_h

#include "GraphicsDevice.h"
#include "Shader.h"
#include "FileSystem.h"


typedef const vector<char>	ShaderCodeType;


class ShaderModules
{
public:
	ShaderModules(GraphicsDevice& graphics);
	ShaderModules(Shaders& shaders, GraphicsDevice& graphics);
	~ShaderModules();

	const StrPtr DEFAULT_NAME_ENTRYPOINT_FUNCTION = "main";

		// MEMBERS
private:
	vector<VkShaderModule>			shaderModules;
	vector<VkShaderStageFlagBits>	shaderStageBits;
	vector<StrPtr>					nameFunctionEntrypoints;

	vector<VkPipelineShaderStageCreateInfo>	shaderStageInfos;
	
	VkDevice	device;
	FileSystem	fileSystem;

		// METHODS
public:
	VkPipelineShaderStageCreateInfo* ShaderStages();
	uint32_t						 NumShaderStages();

private:
	VkShaderModule createShaderModule(ShaderCodeType& code);
	void createShaderStageInfos();

		// getters
public:
};

#endif // ShaderModules_h
