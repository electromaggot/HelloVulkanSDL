//
// jsonConvert_c.h
//	Application Settings - C code in an H file
//
// Convert JSON to/from our app's objects.
//
// Make sure the Project sets this file to NOT COMPILE, because its code is
//	included directly into AppSettings.cpp, which *is* compiled.  The reason
//	is simply to eliminate the need for a separate header file (filled with
//	redundant 'extern' prototypes, which must be continually maintained to
//	match any change to this file), especially when this code or any
//	reference to it will only occur once, in that one other file.
//
// Created 6/10/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "json.hpp"
using json = nlohmann::json;

#include "AppSettings.h"


// AppSettings atomics

void from_json(const json& jsonAppSettings, AppSettings& appSettings)
{
	appSettings.startingWindowWidth	 = jsonAppSettings["startingWindowWidth"];
	appSettings.startingWindowHeight = jsonAppSettings["startingWindowHeight"];
	appSettings.startingWindowX		 = jsonAppSettings["startingWindowX"];
	appSettings.startingWindowY		 = jsonAppSettings["startingWindowY"];
}

void to_json(json& jsonAppSettings, const AppSettings& settings)
{
	jsonAppSettings = json{
		{ "startingWindowWidth",  settings.startingWindowWidth },
		{ "startingWindowHeight", settings.startingWindowHeight },
		{ "startingWindowX", settings.startingWindowX },
		{ "startingWindowY", settings.startingWindowY }
	};
}
