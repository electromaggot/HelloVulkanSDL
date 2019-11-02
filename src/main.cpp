//
// main.cpp
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "HelloTriangle.h"

#include "AppConstants.h"


int main(int argc, char* argv[])
{
	HelloApplication app;

	AppConstants.setExePath(argv[0]);
	std::cout << "PATH: " << AppConstants.getExePath() << std::endl;

	try {
		app.Init();
		app.Run();
	} catch (const exception& e) {
		const char* message = e.what();
		app.DialogBox(message);
		std::cerr << message << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
