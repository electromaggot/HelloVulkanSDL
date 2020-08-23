//
// Local FileSystem.h
//	Project-specific Microsoft-centric
//
// Windows applications install to their own directories under "Program Files" or
//	"Program Files (x86)" into which ".config" types of files may either reside
//	alongside the executable, or in a more "Windows standard" and user-independent
//	fashion: in the "AppData" folder under the \Users directory, each within an
//	app-specific subfolder.
//	This file rides along in VCS but is only include in Microsoft/VisualStudio projects.
//
// 5/19/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanPlatform.h"			// for std::string etc.
#include "AppConstants.h"			// necessary for path of executable

#include <filesystem>				// for fs::create_directories
namespace fs = std::filesystem;		//	(C++17 feature)


static string localPath;


class LocalFileSystem
{
protected:	// (discourage standalone (non-derived) instantiation)

	// Use path of executable (from argv[0] in main()) removing executable name.
	//
	static string exeAccompaniedFullPath(const string& fileName, const char* subdirectoryName)
	{
		string resultPath = AppConstants.getExePath();
		size_t iLastSlash = resultPath.find_last_of('\\', string::npos) + 1;
																	   // ^ include trailing slash itself
		return resultPath.substr(0, iLastSlash) + subdirectoryName + fileName;
	}

	// Use environment to return path for what is probably:  ~\AppData\Roaming\
	//	backslash-terminated, creating directories for first time if they don't exist.
	//
	static string appLocalStorageDirectory()
	{
		if (!localPath.empty())
			return localPath;

		const string appDataDir = "\\AppData\\Roaming\\";
		size_t	numWrit;
		rsize_t	bufSize = 160;
		char	buffer[bufSize];

		errno_t err = getenv_s(&numWrit, buffer, bufSize, "HOMEPATH");
		if (err) {
			err = getenv_s(&numWrit, buffer, bufSize, "HOME");
			if (err)
				return "";	// indicate error: no such environment variable found
		}
		localPath = buffer;
		localPath += appDataDir + AppConstants.CompanyName + "\\" + AppConstants.ProjectName + "\\";

		try {
			fs::create_directories(localPath);	// throws on failure, returns true on "was created" (regardless of "if exists")

			//Log(RAW, "STORAGE %s", localPath.c_str());					//TJ_TODO: causes infinite loop!
			return localPath;
		}
		catch(exception e) {
			//Log(ERROR, "Failed to create directories: %s", localPath);	//TJ_TODO: again, can't log from here!
			return "";	// indicate error: path assembled above invalid for storage
		}
	}
};
