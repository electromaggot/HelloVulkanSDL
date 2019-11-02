//
// PlatformSDL.h
//	General App Chassis
//
// Platform-specific Concretion for Simple DirectMedia Layer.
//
// Feel free to delete the base class/interface if you'll only ever use SDL.
//	An abstraction with one concretion is unnecessary cruft; simpler is better!
//
// TODO: Add multi-monitor (multi-window) support, eventually.
//
// Created 2/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef PlatformSDL_h
#define PlatformSDL_h

// Avoid SDL's many warnings: Empty paragraph passed to '\param' command
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL.h>
#include <SDL_vulkan.h>

#include "iPlatform.h"
#include "ImageSDL.h"


class PlatformSDL : public iPlatform
{
public:
	PlatformSDL();
	~PlatformSDL();

		// MEMBERS
private:
	SDL_Window* pWindow;	// Note: only supports a single window, hence single monitor.

	SDL_Event	event;

	ImageSDL	image;

		// METHODS
public:
	void CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface);
	bool GetWindowSize(int& pixelWidth, int& pixelHeight);
	void DialogBox(const char* message, const char* title = "ERROR", AlertLevel level = FAILURE);
	bool PollEvent();
	bool IsEventQUIT();
	void AwaitEvent();
	void ClearEvents();
	iImageSource& ImageSource()	 { return static_cast<iImageSource&>(image); }
private:
	void initializeSDL();
	void createVulkanCompatibleWindow();
	void querySupportedVulkanExtensions();
	void recordWindowSize();
	float getDisplayScaling();
	float getDisplayDPI(int iDisplay = 0);

	// WIP, TODO: "more officially" integrate later
	void createMultiMonitorWindows();
	void createVulkanSurface(int iScreen, VkInstance instance, VkSurfaceKHR& surface);
	void destroyMultiMonitorWindows();
};

#endif	// PlatformSDL_h
