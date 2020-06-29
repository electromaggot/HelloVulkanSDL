//
// AppSettings.cpp
//	Application Properties/Attributes
//
// See header file comment for overview.
// Implementation-side of AppSettings.  Isolate Attributes
//	and their values from (JSON-specific) delivery mechanism.
//
// Created 6/10/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "AppSettings.h"

#include "VulkanPlatform.h"
#include "AppConstants.h"

#include <iostream>			//
#include <fstream>			// basic file operations
using namespace std;		//


#include "jsonConvert.c"	// directly pull-in json methods (this is
							//	the only place they'll get compiled)
json jsonSettings;


AppSettings::AppSettings()
{
	try {
		Retrieve();
	}
	catch (exception& ex) {
		Log(ERROR, "AppSettings JSON module threw: %s", ex.what());
	}
}


void AppSettings::Save()
{
	jsonSettings = *this;		// (this does indeed work! although not in reverse)

	ofstream	settingsFile;
	settingsFile.open(AppConstants.SettingsFileName);
	if (settingsFile.is_open())
	{
		settingsFile << setw(2) << jsonSettings << endl;
		settingsFile.close();
	}
}

void AppSettings::Retrieve()
{
	try {
		json jsonRetrieved;

		ifstream settingsFile;
		settingsFile.open(AppConstants.SettingsFileName);
		if (settingsFile.is_open())
		{
			settingsFile >> jsonRetrieved;
			settingsFile.close();
		}
		else {
			Log(NOTE, "File \"%s\" not found, using default settings.", AppConstants.SettingsFileName);
			return;
		}

		startingWindowWidth	 = jsonRetrieved["startingWindowWidth"];
		startingWindowHeight = jsonRetrieved["startingWindowHeight"];
		startingWindowX		 = jsonRetrieved["startingWindowX"];
		startingWindowY		 = jsonRetrieved["startingWindowY"];

		jsonSettings = jsonRetrieved;
	}
	catch (exception& ex) {
		Log(ERROR, "Retrieve() JSON parse threw: %s", ex.what());
		Log(NOTE,  "Using default settings and abandoning saved ones.");
	}
}
