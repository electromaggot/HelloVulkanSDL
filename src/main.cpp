//
// main.cpp
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"

#include "AppConstants.h"
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
