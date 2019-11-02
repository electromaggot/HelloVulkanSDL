//
// Local FileSystem.cpp
//	Project-specific Apple-centric
//
// Remember to put:  extern string PlatformSpecificFullPath(const string& fileName, const char* subdirectoryName);
//	at the top of whomever uses this.
//
// Apple platforms have certain quirks/conventions
//	for accessing files baked into an .app or bundle.
//	Handle those here.  This file rides along in VCS
//	but is only include in Apple/Xcode projects.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanPlatform.h"		// for std::string etc.

#include <TargetConditionals.h>	// for TARGET_OS_IPHONE

#include "AppConstants.h"	// necessary: path of executable


string PlatformSpecificFullPath(const string& fileName, const char* subdirectoryName)
{
#if TARGET_OS_IPHONE	// Do not use #ifdef here! It and TARGET_IPHONE_SIMULATOR will be 0 or 1. TARGET_OS_MAC is always 1.
	return subdirectoryName + fileName;
#elif TARGET_OS_MAC
	// ExePath will be .../Contents/MacOS/<appName> so "remove" the last two components
	auto exePath = AppConstants.getExePath();
	auto iRightSlash = strlen(exePath) - 1;
	while (iRightSlash > 0 && exePath[iRightSlash--] != '/');	// (do twice)
	while (iRightSlash > 0 && exePath[iRightSlash] != '/') --iRightSlash;
	string contentsPath(exePath);
	if (iRightSlash > 0)
		contentsPath.resize(iRightSlash);
	auto fullPath = contentsPath + "/Resources/" + subdirectoryName + fileName;
	return fullPath;
	// example: "HelloTriangle-macOS.app/Contents/Resources/compiledShaders/" + shaderFilename);
#else
	return subdirectoryName + fileName;
#endif
}
