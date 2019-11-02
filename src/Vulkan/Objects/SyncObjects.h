//
// SyncObjects.h
//	Vulkan Setup
//
// Encapsulate semaphores/fences that coordinate
//	render completion with frame presentation.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef SyncObjects_h
#define SyncObjects_h

#include "GraphicsDevice.h"


#define MAX_FRAMES_IN_FLIGHT	2	//TJ_TODO: determine if better elsewhere
									// OR if this should be dynamic e.g. = swapchain.getImageViews.size()

class SyncObjects
{
public:
	SyncObjects(GraphicsDevice& graphics);
	~SyncObjects();

		// MEMBERS
public:
	const int MaxFramesInFlight = MAX_FRAMES_IN_FLIGHT;

public:		//TJ: the following are "tentatively" public; determine later if they can be made less visible
	VkSemaphore	imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore	renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkFence		inFlightFences[MAX_FRAMES_IN_FLIGHT];

private:
	VkDevice&	device;		// Save to destruct as constructed

		// METHODS
private:
	void createSyncObjects();
};

#endif // SyncObjects_h
