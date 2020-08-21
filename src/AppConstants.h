//
// AppConstants.h
//	VulkanSDL Application Chassis
//
// Created 2/17/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef AppConstants_h
#define AppConstants_h

#ifdef INSTANTIATE
  #define extern
#endif

#include "PlatformConstants.h"
#include "VulkanPlatform.h"
#include "AppSettings.h"


extern struct Constants
{
	const StrPtr AppName			= "HelloVulkanSDL";
	const uint32_t AppVersion		= VK_MAKE_VERSION(1, 0, 0);		// (Vulkan wants this)

	const StrPtr WindowTitle		= "Vulkan Demo";

	const StrPtr SettingsFileName	= "Settings.json";

	const StrPtr DebugLogFileName	= "DebugLog.txt";

	const StrPtr CompanyName		= "GitHubProject";
	const StrPtr ProjectName		= "HelloTriangle";

#define COMPARE_AGAINST		IPHONE_X		// These comparison cases are for
#if COMPARE_AGAINST == SHADERTOY_16x9		//	test and are all temporary!
	const int DefaultWindowWidth	= 640;
	const int DefaultWindowHeight	= 360;
#elif COMPARE_AGAINST == IPHONE_X
	const int DefaultWindowWidth	= 812;
	const int DefaultWindowHeight	= 375;
#elif COMPARE_AGAINST == WINDOW_5x4
	const int DefaultWindowWidth	= 1280;
	const int DefaultWindowHeight	= 1024;
#else				 // WINDOW_16x9
	const int DefaultWindowWidth	= 1280;
	const int DefaultWindowHeight	= 720;
#endif
	// NOTE on WINDOW SIZE - which, even for a full-screen app on a mobile device, is still
	//						 considered its "display window" - the surface the app renders to...
	// The above define some starting sizes (and as you can see, per-platform), but as for the
	//	actual "window size" variables themselves - and their definitive value* - those remain
	//	PLATFORM-DEPENDENT, therefore reside in the iPlatform and OS Abstraction classes.
	// * - window size may subsequently change from values above, if app implements a means to
	//	save/retrieve size, or as user interacts via drag-resize, maximize, rotate device, etc.

	const int MaxSaneScreenWidth	= 7680 * 2;		// 8K x 2 values. Simply sanity-
	const int MaxSaneScreenHeight	= 4320 * 2;		//	check on pixel-related ranges.

	VkClearValue DefaultClearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };

	const bool SupportStereo3D		= false;


	StrPtr	getExePath() const	 {	return exePath; }

	void	setExePath(StrPtr p) {	if (exePath == nullptr)
		/* Only allow set once! */		exePath = p;		}

private:
	StrPtr	exePath					= nullptr;

public:
	AppSettings	 Settings;			// singleton, instantiate LAST: uses
									//		constants initialized above
} AppConstants;


#ifdef extern
  #undef extern
#endif

#endif // AppConstants_h


/*       1         2         3         4         5         6         7         8         9        10        11        12
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
