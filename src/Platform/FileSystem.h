//
// FileSystem.h
//	Vulkan Platform Abstraction
//
// Provide OS-like file system functionality in
//	a platform-independent manner.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef FileSystem_h
#define FileSystem_h

#include "VulkanPlatform.h"


class FileSystem
{
		// MEMBERS
private:
	vector<char> buffer;

		// METHODS
public:
	static string ShaderFileFullPath(StrPtr fileName);
	static string TextureFileFullPath(StrPtr fileName);

	vector<char> ReadShaderFile(const string& shaderFilename);
	vector<char> ReadTextureFile(const string& imageFilename);
private:
	vector<char> readFile(const string& fileName, const char* subdirectoryName);
	vector<char> readFile(const string& pathName);
};

#endif // FileSystem_h
