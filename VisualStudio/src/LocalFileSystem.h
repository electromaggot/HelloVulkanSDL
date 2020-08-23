//
// Local FileSystem.cpp
//	Project-specific Microsoft-centric
//
// Remember to put:  extern string PlatformSpecificFullPath(const string& fileName, const char* subdirectoryName);
//	at the top of whomever uses this.
//
// Apple platforms have certain quirks/conventions
//	for accessing files baked into an .app or bundle.
//	Handle those here.  This file rides along in VCS
//	but is only include in Microsoft/VisualStudio projects.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanPlatform.h"		// for std::string etc.

#include "AppConstants.h"	// necessary: path of executable


string PlatformSpecificFullPath(const string& fileName, const char* subdirectoryName)
{
	return subdirectoryName + fileName;
}
