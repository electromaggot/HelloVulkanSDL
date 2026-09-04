//
// main.cpp
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"

#define INSTANTIATE			// This translation unit allocates storage for AppConstants (and,
	#include "AppConstants.h"	//	via it, PlatformConstants).  It belongs here, in the app
#undef INSTANTIATE			//	that owns them -- VulkanModule used to do it by side effect.
#include "Logging.h"


int main(int argc, char* argv[])
{
	AppConstants.setExePath(argv[0]);
	LogStartup();

	HelloApplication app;

	try {
		app.Init();
		app.Run();
	} catch (const exception& e) {
		const char* message = e.what();
		app.DialogBox(message);
		Log(RAW, "FAIL: %s", message);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
