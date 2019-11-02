//
// PlatformGLFW.h
//	General App Chassis
//
// Platform-specific Concretion for Graphics Library FrameWork.
//
// Created 2/13/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef PlatformGLFW_h
#define PlatformGLFW_h

#include "iPlatform.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PlatformGLFW : public iPlatform
{
public:
	PlatformGLFW();
	~PlatformGLFW();

	// MEMBERS
private:
	GLFWwindow* pWindow;

	// METHODS
public:
	void CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface);
	bool GetWindowSize(int& pixelWidth, int& pixelHeight);
	void DialogBox(const char* message, const char* title = "ERROR", AlertLevel level = ERROR);
	bool PollEvent();
	bool IsEventQUIT();
	void AwaitEvent();
	void ClearEvents() {};
private:
	void initWindow();
	bool findAvailableVulkanExtensions();
	std::vector<const char*> queryRequiredExtensions();
	string errorMessage();

	static void framebufferResizeCallback(GLFWwindow* pWindow, int width, int height);
	static void windowIconifyCallback(GLFWwindow* pWindow, int isIconified);
	static void mouseMotionCallback(GLFWwindow* pWindow, double xpos, double ypos);
};

#endif	// PlatformGLFW_h
