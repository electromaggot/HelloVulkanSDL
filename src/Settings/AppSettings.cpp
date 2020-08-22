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
#include "FileSystem.h"

#include "jsonConvert_c.h"	// directly pull-in json methods (this is
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

	if (jsonSettings.empty() || jsonSettings.type() == value_t::null)
		return;

	string fileName = AppConstants.SettingsFileName;
	fileName = FileSystem::AppLocalStorageDirectory() + fileName;
	const char* filePath = fileName.c_str();
	Log(RAW, "%s", filePath);

	ofstream	settingsFile;
	settingsFile.open(filePath);
	if (settingsFile.is_open())
	{
		settingsFile << std::setw(2) << jsonSettings << endl;
		settingsFile.close();
	}
}

void AppSettings::Retrieve()
{
	try {
		json jsonRetrieved;

		string fileName = AppConstants.SettingsFileName;
		fileName = FileSystem::AppLocalStorageDirectory() + fileName;
		const char* filePath = fileName.c_str();

		ifstream settingsFile;
		settingsFile.open(filePath);
		if (settingsFile.is_open())
		{
			settingsFile >> jsonRetrieved;
			settingsFile.close();
		}
		else {
			Log(LOW, "File \"%s\" not found, using default settings.", AppConstants.SettingsFileName);
			return;
		}
		if (jsonRetrieved.empty() || jsonRetrieved.type() == value_t::null) {
			Log(ERROR, "File \"%s\" empty, corrupt, or not JSON as expected.", AppConstants.SettingsFileName);
			return;
		}
		void jsonKeyToInt(const char* key, int& intTo, json& jsonFrom);

		jsonKeyToInt("startingWindowWidth",	 startingWindowWidth,  jsonRetrieved);
		jsonKeyToInt("startingWindowHeight", startingWindowHeight, jsonRetrieved);
		jsonKeyToInt("startingWindowX",		 startingWindowX,	   jsonRetrieved);
		jsonKeyToInt("startingWindowY",		 startingWindowY,	   jsonRetrieved);

		if (startingWindowWidth <= 0 || startingWindowHeight <= 0) {
			Log(ERROR, "File \"%s\" lacks CRITICAL startup values.", AppConstants.SettingsFileName);
			return;
		}
		isInitialized = true;

		jsonSettings = jsonRetrieved;
	}
	catch (exception& ex) {
		Log(ERROR, "Retrieve() JSON parse threw: %s", ex.what());
		Log(NOTE,  "Using default settings and abandoning saved ones.");
	}
}

inline void jsonKeyToInt(const char* key, int& intTo, json& jsonFrom) {
	try {
		intTo = jsonFrom[key];
	}
	catch (exception& ex) {
		Log(ERROR, "json[%s] unresolved, using default. (%s)", key, ex.what());
	}
}
