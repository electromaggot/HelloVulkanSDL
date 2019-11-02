//
// PlatformExtension.h
//	Vulkan App Chassis
//	contains implementation (only #include this .h once from the .cpp where needed)
//
// Extend Vulkan's platform further.
// That is, to the Platform Extensions, request additional extensions.
//
// Regarding VK_EXT_debug_report, it is:
//	- the main reason for this file, although other VK_EXT's may be added.
//	- seen as platform-dependent, because the debug messages must go somewhere,
//	  determined at the platform level, be that a log, the console, a window, etc.
//	Note that even in 2019, MoltenVK (i.e. Vulkan on Apple) doesn't support
//	Validation Layers, so check before trying to add them.  For reference:
//		https://moltengl.com/forums/topic/support-for-vk_ext_debug_report-instance-extension/
//	Otherwise, something like the following throws, which this class will prevent.
//	  [***MoltenVK ERROR***] VK_ERROR_EXTENSION_NOT_PRESENT: Vulkan extension VK_EXT_debug_report is not supported.
//
// Created 3/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef PlatformExtension_h
#define PlatformExtension_h

#include "iPlatform.h"
#include "VulkanConfigure.h"
#include "PlatformSpecifics.h"	// (for __emplace_back on iOS, possibly)

#include <vector>
#include <iostream>				// for uint


class PlatformExtension {

	VkExtensionProperties*	allExtensions;
	ArrayCount				nAllExtensions = 0;

	vector<StrPtr> grantedExtensionNames;

public:
			// getters
	ArrayCount	nRequestedExtensionsAvailable()	{ return (ArrayCount) grantedExtensionNames.size(); }
	StrPtr*		requestedExtensionNames()		{ return grantedExtensionNames.data(); }


	// Just because an extension is requested, doesn't mean it's present or supported (or spelled
	//	right, although the VK_EXT_..._EXTENSION_NAME macros help with that).  So query
	//	vkEnumerateInstanceExtensionProperties() first to see if it's in that list.
	//
	PlatformExtension()
	{
		Log(NOTE, "----V-U-L-K-A-N---S-E-T-U-P---S-T-A-R-T----");	// (platform does initialize first!)
		sanityCheckVulkanConfiguration();

		vkEnumerateInstanceExtensionProperties(nullptr, &nAllExtensions, nullptr);

		allExtensions = new VkExtensionProperties[nAllExtensions];
		vkEnumerateInstanceExtensionProperties(nullptr, &nAllExtensions, allExtensions);

		showAllExtensions();

		for (int iReq = 0; iReq < N_INSTANCE_EXTENSION_NAMES; ++iReq) {
			StrPtr name = INSTANCE_EXTENSION_NAMES[iReq];
			bool unmatched = true;
			for (Index iExtns = 0; iExtns < nAllExtensions; ++iExtns) {
				if (strcmp(name, allExtensions[iExtns].extensionName) == 0) {
					grantedExtensionNames.emplace_back(name);
					unmatched = false;
					break;
				}
			}
			if (unmatched) {
				if (REQUIRE_INSTANCE_EXTENSION[iReq])
					Fatal("Required Instance Extension NOT Supported: " + string(name));
				else
					Log(WARN, "Requested Extension NOT Supported: " + string(name));
			}
		}
	}

	~PlatformExtension()
	{
		delete allExtensions;
	}


	bool IsSupported(StrPtr extensionName)
	{
		for (size_t iExtension = 0; iExtension < nAllExtensions; ++iExtension)
			if (strcmp(extensionName, allExtensions[iExtension].extensionName) == 0)
				return true;
		return false;
	}

	bool IsDebugReportSupported()
	{
		return IsSupported(DEBUG_REPORT_EXTENSION);
	}

private:
	void showAllExtensions()
	{
		Log(NOTE, "Vulkan reports Extensions Supported: %d", nAllExtensions);

		char prefix[8];
		for (Index iExtension = 0; iExtension < nAllExtensions; ) {
			StrPtr eachExtensionName = allExtensions[iExtension++].extensionName;
			snprintf(prefix, 8, " %2u. ", iExtension);
			const char* postfix = "";
			for (StrPtr requestedName : grantedExtensionNames) {
				if (strcmp(requestedName, eachExtensionName) == 0) {
					sprintf(prefix, " --> ");
					postfix = " <-- requested";
					break;
				}
			}
			Log(NOTE, "%s%s%s", prefix, eachExtensionName, postfix);
		}
	}
};


#endif	// PlatformExtend_h


/* DEVELOPER NOTE
	FYI a nice example list of extensions:
		http://hackage.haskell.org/package/vulkan-2.1.0.0/docs/
*/
