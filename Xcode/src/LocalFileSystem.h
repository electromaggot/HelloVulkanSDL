//
// Local FileSystem.h
//	Project-specific Apple-centric
//
// Apple platforms have certain quirks/conventions for accessing files baked into an .app or bundle.
//	Handle those here.  This file rides along in VCS but is only include in Apple/Xcode projects.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanPlatform.h"				// for std::string etc.
#include "AppConstants.h"				// necessary for path of executable
#include <TargetConditionals.h>			// for TARGET_OS_IPHONE
#include <sys/stat.h>					// for stat, mkdir
#include <CoreServices/CoreServices.h>	// for (unfortunately) deprecated FS functions


static char localPath[PATH_MAX];


class LocalFileSystem
{
protected:	// (discourage standalone (non-derived) instantiation)

	// Use path of executable (from argv[0] in main()) removing executable name.
	//	For macOS bundles, also requires backing out of its "/MacOS/" directory.
	//
	static string exeAccompaniedFullPath(const string& fileName, const char* subdirectoryName)
	{
	#if TARGET_OS_IPHONE	// Do not use #ifdef here! It and TARGET_IPHONE_SIMULATOR will be 0 or 1. TARGET_OS_MAC is always 1.
		return subdirectoryName + fileName;
	#elif TARGET_OS_MAC
		// ExePath will be .../Contents/MacOS/<appName> so "remove" the last two components
		auto exePath = AppConstants.getExePath();
		string contentsPath;
		if (strrchr(exePath, '/') == &exePath[1] && exePath[0] == '.') {	// "non-full" path, e.g. just: ./HelloTriangle
			contentsPath = "..";											//	so simply back out of directory: MacOS
		} else {
			contentsPath = exePath;
			auto iRightSlash = strlen(exePath) - 1;
			while (iRightSlash > 0 && exePath[iRightSlash--] != '/');	// (do twice)
			while (iRightSlash > 0 && exePath[iRightSlash] != '/') --iRightSlash;
			if (iRightSlash > 0)
				contentsPath.resize(iRightSlash);
		}
		return contentsPath + "/Resources/" + subdirectoryName + fileName;
		// example: "HelloTriangle-macOS.app/Contents/Resources/compiledShaders/" + shaderFilename);
	#else
		return subdirectoryName + fileName;
	#endif
	}


	inline static void appendTrailingSlash(char* path) {
		size_t iEnd = strlen(path);		// char array must already be null terminated!
		path[iEnd++] = '/';
		path[iEnd] = '\0';
	}
	inline static bool directoryExists(char* atPath) {
		struct stat info;
		return stat(atPath, &info) == 0 && info.st_mode & S_IFDIR;
	}
	inline static bool createDirectoryIfDoesntExist(char* atPath) {
		if (!directoryExists(atPath)) {
			mode_t	mode = 0733;
			int iError = 0;
			iError = mkdir(atPath, mode);
			if (iError) {
				printf("ERROR: mkdir '%s' failed with error number %d", atPath, iError);
				return false;
			}
		}
		return true;	// i.e. directory exists/was created
	}


	// Use Apple-specific but unfortunately deprecated methods to return full path for what is probably:
	//	Creates directories for first time if they don't exist.				~/Library/Application Support/
	//
	static string appLocalStorageDirectory()
	{
		if (*localPath)
			return localPath;

		size_t	pathMax = PATH_MAX - strlen(AppConstants.CompanyName) - strlen(AppConstants.ProjectName) - 4;
																 // additional slashes and null terminator ^
		#if TARGET_OS_IPHONE
			const char* strAppSpt = "/Library/Application Support";
			strncpy(localPath, getenv("HOME"), pathMax);
				// e.g. "/private/var/mobile/Containers/Data/Application/E75C6C71-F872-4098-B1DC-B3ED1B10A6BC"
			pathMax -= strlen(localPath) - strlen(strAppSpt);
			strncat(localPath, strAppSpt, pathMax);
				// note that attempting to use "~/Library/Application Support" on iOS fails
		#else // TARGET_OS_MAC
			FSRef	ref;			// https://stackoverflow.com/a/5188422/376835
			OSType	folderType = kApplicationSupportFolderType;
			// The below methods are deprecated, but in lieu of coding an Obj-C wrapper:
			#pragma clang diagnostic ignored "-Wdeprecated"
			FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);
			FSRefMakePath(&ref, (UInt8*) &localPath, pathMax);
		#endif
		if (strlen(localPath) < pathMax) {		// there's room to append to path
			appendTrailingSlash(localPath);
			strcat(localPath, AppConstants.CompanyName);	// append e.g.: "/GitHubProject"
			if (createDirectoryIfDoesntExist(localPath)) {
				appendTrailingSlash(localPath);
				strcat(localPath, AppConstants.ProjectName);	// append e.g.: "/HelloTriangle"
				if (createDirectoryIfDoesntExist(localPath)) {
					appendTrailingSlash(localPath);
					Log(RAW, "STORAGE %s", localPath);
					return localPath;
				}
			}
		}
		return "";	// indicate error: path assembled above invalid for storage
	}
};



/* DEV NOTE
	For directory operations, could have done:
#include <filesystem>	// for fs::create_directories
namespace fs = std::__fs::filesystem;
	and then
fs::create_directories(path);
	with all pathnames appended at once, simplifying
	the above code...

   However, <filesystem> is a C++17 feature and didn't seem
	available until macOS 10.15 Catalina.  To not limit to that
	(and stick with original 10.13 target) we instead use the
	standard library calls, despite additional complexity.
*/
