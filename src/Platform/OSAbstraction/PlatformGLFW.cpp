//
// PlatformGLFW.cpp
//	General App Chassis
//
// See matched header file for definitive main comment.
//
// Created 2/13/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "PlatformGLFW.h"


//TJ_TODO: TJ_TEMPORARY_NOTE: This probably won't work, let alone build; needs testing on another platform.


PlatformGLFW::PlatformGLFW()
{
	namePlatform = "GLFW";

	initWindow();

	findAvailableVulkanExtensions();
}

PlatformGLFW::~PlatformGLFW()
{
	glfwDestroyWindow(pWindow);

	glfwTerminate();
}


// INITIALIZATION

void PlatformGLFW::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	GetWindowSize(pixelsWide, pixelsHigh);

	glfwSetWindowUserPointer(pWindow, this);
	glfwSetFramebufferSizeCallback(pWindow, PlatformGLFW::framebufferResizeCallback);
	glfwSetWindowIconifyCallback(pWindow, PlatformGLFW::windowIconifyCallback);
	glfwSetCursorPosCallback(pWindow, PlatformGLFW::mouseMotionCallback);
}

void PlatformGLFW::CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface)
{
	call = glfwCreateWindowSurface(instance, pWindow, nullptr, &surface);
	if (call != VK_SUCCESS)
		Fatal("GLFW Create Window Surface" + ErrStr(call) + ", " + errorMessage();
}

// Returns window dimensions in pixels via pointed-to 'int's. "If an error occurs,
//	all non-NULL size arguments will be set to zero." per:
// https://www.glfw.org/docs/latest/group__window.html#gaeea7cbc03373a41fb51cfbf9f2a5d4c6
//
bool PlatformGLFW::GetWindowSize(int& pixelWidth, int& pixelHeight)
{
	glfwGetFramebufferSize(window, &pixelWidth, &pixelHeight);	// (void return value)

	if (pixelWidth != 0 || pixelHeight != 0)
		return true;
	Log(ERROR, "GLFW Get Framebuffer Size: " + errorMessage());
	return false;
}

bool PlatformGLFW::findAvailableVulkanExtensions()	//TJ: CONSOLIDATE THIS
{
	foundVulkanExtensions = queryRequiredExtensions();

	return (foundVulkanExtensions.size() > 0);
}

std::vector<const char*> PlatformGLFW::queryRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

string errorMessage()
{
	const char* description;
	int code = glfwGetError(&description);

	return string(description) + " (" + to_string(code) + ")";
}


// CALLBACKS

void PlatformGLFW::framebufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
	auto self = reinterpret_cast<PlatformGLFW*>(glfwGetWindowUserPointer(pWindow));
	self->GetWindowSize(pixelsWide, pixelsHigh);
	self->IsWindowResized = true;	// (note this remains set until retrieved, whence one-shot resets it)
}

// Avoid repeated (polled) calls to glfwGetWindowAttrib(pWindow, GLFW_ICONIFIED)
//	by only setting the boolean when window is actually either iconified or restored.
//	FYI: glfwGetFramebufferSize(pWindow, &width, &height); isMinimized = (width == 0 || height == 0);
//	(e.g. https://vulkan-tutorial.com/en/Drawing_a_triangle/Swap_chain_recreation#page_Handling-minimization )
//	...is another suggested approach, but the code below couldn't be simpler and can't be beat.
//
void PlatformGLFW::windowIconifyCallback(GLFWwindow* pWindow, int isIconified)
{
	auto self = reinterpret_cast<PlatformGLFW*>(glfwGetWindowUserPointer(pWindow));
	self->IsWindowMinimized = isIconified ? true : false;
}

void PlatformGLFW::mouseMotionCallback(GLFWwindow* pWindow, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
}


// UTILITY

// Inform user via pop-up window.  NOT "FULLY" IMPLEMENTED.
//
void PlatformGLFW::DialogBox(const char* message, const char* title, AlertLevel level)
{
	// GLFW doesn't provide any form of message box, per:
	//	https://www.glfw.org/faq.html#27---will-message-box-support-be-added-to-glfw
	// (SDL does, as mentioned here:  (tl;dr: rolling its own on *NIX/X11 systems)
	//	 https://wiki.libsdl.org/SDL_ShowSimpleMessageBox )

	// So simply dump to the console/debug log/error out:
	const char* levels[] = { "ERROR", "WARNING", "INFORMATION" };
	cerr << levels[level] << ": " << title << " - " << message << endl;
}


// RUNTIME

bool PlatformGLFW::PollEvent()
{
	glfwPollEvents();
	return true;
}

bool PlatformGLFW::IsEventQUIT()
{
	return glfwWindowShouldClose(pWindow);
}

void PlatformGLFW::AwaitEvent()
{
	glfwWaitEvents();
}
