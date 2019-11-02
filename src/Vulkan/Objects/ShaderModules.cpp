//
// ShaderModules.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "ShaderModules.h"
#include "PlatformSpecifics.h"	// (for __emplace_back on iOS, possibly)


ShaderModules::ShaderModules(GraphicsDevice& graphics)
	:	device(graphics.getLogical())
{ }

ShaderModules::ShaderModules(Shaders& shaders, GraphicsDevice& graphics)
	:	ShaderModules(graphics)
{
	for (auto& shader : shaders )
	{
		ShaderCodeType shaderCode = fileSystem.ReadShaderFile(shader.nameShaderFile);

		VkShaderModule shaderModule = createShaderModule(shaderCode);

		StrPtr entrypointFunctionName = shader.nameEntrypointFunction;
		if (entrypointFunctionName == nullptr)
			entrypointFunctionName = DEFAULT_NAME_ENTRYPOINT_FUNCTION;

		shaderModules.emplace_back(shaderModule);
		shaderStageBits.emplace_back(static_cast<VkShaderStageFlagBits>(shader.fileType));
		nameFunctionEntrypoints.emplace_back(entrypointFunctionName);
	}
}

ShaderModules::~ShaderModules()
{
	for (auto& shaderModule : shaderModules)
		vkDestroyShaderModule(device, shaderModule, nullALLOC);
}


VkShaderModule ShaderModules::createShaderModule(ShaderCodeType& code)
{
	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};
	VkShaderModule shaderModule;

	call = vkCreateShaderModule(device, &createInfo, nullALLOC, &shaderModule);

	if (call != VK_SUCCESS)
		Fatal("Create Shader Module FAILURE" + ErrStr(call));
	return shaderModule;
}

VkPipelineShaderStageCreateInfo* ShaderModules::ShaderStages()
{
	createShaderStageInfos();

	return shaderStageInfos.data();
}

// The number of Shader Stages always equals the number of Shader Modules,
//	without depending on createShaderStages() to be called beforehand.
//
uint32_t ShaderModules::NumShaderStages()
{
	return static_cast<uint32_t>(shaderModules.size());
}

void ShaderModules::createShaderStageInfos()
{
	size_t nShaders = shaderModules.size();

	shaderStageInfos.resize(nShaders);

	for (size_t iShader = 0; iShader < nShaders; ++iShader)
		shaderStageInfos[iShader] = {
			.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext	= nullptr,
			.flags	= 0,
			.stage	= shaderStageBits[iShader],
			.module	= shaderModules[iShader],
			.pSpecializationInfo = nullptr,
			.pName	= nameFunctionEntrypoints[iShader]
		};
}
