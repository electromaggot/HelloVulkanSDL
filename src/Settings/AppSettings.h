//
// AppSettings.h
//	Application Properties/Attributes
//
// Define, assign, and persist Settings values to JSON file (which is optional*).
//	One thing for which this is convenient, for example: saving/retrieving window
//	location/size on desktop platforms, as window is moved or resized.
//
// * - Relies on  json.hpp  which is stubbed-out when this file originally distributes.
//	So if you want this capability but see a message like:
//		Note: Save Window Geometry: FAILED; No JSON handling. See: AppSettings.h + json.hpp
//  Do what it says, open/read the  json.hpp  and follow instructions there to replace
//	that file with the open-source non-stub version to add this functionality.
//
// Additional implementation-related detail is at end of this header file.
//
// Created 6/10/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef AppSettings_h
#define AppSettings_h

#include "iAppSettings.h"		// VulkanModule's view of what it needs from us

#include <string>


class AppSettings : public iAppSettings
{
public:
	AppSettings();

	void Save() override;
	void Retrieve();

		// iAppSettings -- what VulkanModule reads and writes back (window geometry, mainly).
		//	These simply adapt the plain members below, which are what actually persist.
	WindowGeometry GetWindowGeometry() const override
	{
		return { startingWindowX, startingWindowY,
				 startingWindowWidth, startingWindowHeight,
				 isFullScreen, isInitialized };
	}
	void SetWindowGeometry(const WindowGeometry& geometry) override
	{
		startingWindowX		 = geometry.x;
		startingWindowY		 = geometry.y;
		startingWindowWidth	 = geometry.width;
		startingWindowHeight = geometry.height;
		isFullScreen		 = geometry.isFullScreen;
		isInitialized		 = geometry.isStored;
	}
	bool   IsDebugLogToFile()  const override	{ return isDebugLogToFile; }
	StrPtr SettingsFilePath()  const override	{ return filePath.c_str(); }

	std::string filePath;

	bool isInitialized = false;

	// APP/GENERAL SETTINGS

	bool isDebugLogToFile = true;

	// WINDOWING-SYSTEM/GRAPHICS STARTUP SETTINGS

	int startingWindowWidth	 = 0;
	int startingWindowHeight = 0;
	int startingWindowX		 = 0;
	int startingWindowY		 = 0;

	bool isFullScreen = false;		// Must actually be PERSISTED (see Save/Retrieve below)
};

#endif	// AppSettings_h


// DEV NOTE
//	  Design considerations for persisting values to JSON, in general,
//		and for this class in particular...
// To encapsulate:  (design/needs/choices such as:)
//	- "Delivery" of universal (app-wide) values that may be needed anywhere.
//	- Security (risk of someone changing) vs. ease-of-access/speed-of-access.
//	- Persist of Attributes:
//	  - First of all, should settings persist? or always assume default values.
//	  - How?  File or hard-coded (testing) data or over network/cloud (*).
//	  - Format?  JSON (probably ever "only choice" but in theory, encapsulated).
//	  - Specific choice of which JSON Parser (i.e. if we decide to go with
//		a different one later, only this class is impacted).
//	  - Encryption layer possible (for e.g. a password).
//	  - (*) - Specific HTTP library chosen.
// There must be strong differentiation between Platform variables and App settings.
//	The latter may include default values that are app-arbitrary, but the former
//	are operational.  However, this class can still be used to "deliver" those.
// Interfaces with either File or Network (HTTP) component to abstract
//	destination for storage/retrieval.
