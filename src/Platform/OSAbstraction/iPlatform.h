//
// iPlatform.h
//	Platform "Interface"/Base Class (combined)
//	General App Chassis		Vulkan-centric
//
// Platform ABSTRACTION
//	from which erect platform-specific concretions.
//	Platform layers are either direct native/OS interfaces
//	or projects that themselves abstract I/O such as SDL, GLFW, etc.
//
// Created 2/4/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef iPlatform_h
#define iPlatform_h

#include "VulkanPlatform.h"
#include "PlatformExtension.h"
#include "iImageSource.h"


class iPlatform
{
public:
	enum AlertLevel { FAILURE, WARNING, INFORMATION };

		// MEMBERS
protected:
	StrPtr*		supportedVulkanExtensions;	// supported by the given platform
	ArrayCount	nVulkanExtensions;
	ArrayCount	nAdditionalExtensions;

	PlatformExtension extendedPlatform;

	StrPtr		namePlatform;

	bool		isWindowResized   = false;
	bool		isWindowMinimized = false;

public:
	int			pixelsWide = 0;		// Publicly-vulnerable but low-overhead convenience values,
	int			pixelsHigh = 0;		//	especially if read every frame, avoid continuous heavy calls
									//	to GetWindowSize() if those values only change upon resize.
	int			mouseX = 0;
	int			mouseY = 0;			// and make these easy access for continual pull too.
									//	("making these public" risk assessment: if someone writes to change these, it'll be fleeting)
		// METHODS
public:
	virtual void CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface) = 0;
				// should throw fatally on failure

	virtual bool GetWindowSize(int& pixelWidth, int& pixelHeight) = 0;	// false on failure
				 // in pixels, because that's what's being rendered to

	virtual void DialogBox(const char* message, const char* title, AlertLevel level) = 0;

	virtual bool PollEvent()	= 0;		// return false if failure to poll
	virtual bool IsEventQUIT()	= 0;
	virtual void AwaitEvent()	= 0;
	virtual void ClearEvents()	= 0;

	virtual iImageSource& ImageSource() = 0;

		// getters
public:
	StrPtr*		getSupportedVulkanExtensions()	{ return supportedVulkanExtensions; }
	ArrayCount	getVulkanExtensionCount()		{ return nVulkanExtensions; }

	bool		IsWindowResized()				{ bool wasWindowResized = isWindowResized;
				/* This is a ONE SHOT! */		  isWindowResized = false;
				/*	Only resets once read. */	  return wasWindowResized; }
	bool		IsWindowMinimized()				{ return isWindowMinimized; }

	bool		IsDebugReportProvided()			{ return extendedPlatform.IsDebugReportSupported(); }

		// base method implementation
protected:
	void displayFoundVulkanExtensions()
	{
		auto nExtensions = getVulkanExtensionCount();
		const char* appended = nAdditionalExtensions > 0 ? " (requests appended)" : "";
		Log(NOTE, "Platform (%s) reports Vulkan INSTANCE EXTENSIONS: %d %s", namePlatform, nExtensions, appended);
		for (Index iExt = 0; iExt < nExtensions; ++iExt) {
			StrPtr sdlReportedExtension = supportedVulkanExtensions[iExt];
			StrPtr actuallySupported = extendedPlatform.IsSupported(sdlReportedExtension) ? " (supported)" : "";
			Log(NOTE, " %2lu. %s%s", iExt + 1, sdlReportedExtension, actuallySupported);
		}
	}
};


#endif	// iPlatform_h


/* DEVELOPER NOTE - design decision
For storing the array of Vulkan extension names, we went with an array of simple
 character pointers instead of, say, vector<string>.  This isn't so much about
 std::vector's overhead.  It's more about Vulkan's (and SDL's in this case)
 getters following this repeated pattern:
 	uint32_t nItemsInArray;
	VkGetArrayOfSomething(&nItemsInArray, NULL);			// gets "number of" items in array
	ArrayType pArray = <code to allocate array of "n" items>
	VkGetArrayOfSomething(&nItemsInArray, pArray);			// fill the array with data
 Here it's easy to just keep the "number of" variable around, that is, instead
 of a local variable, make it a member variable.
So you'll see this practice repeated elsewhere, like in the Vulkan module.
*/
