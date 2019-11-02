//
// SyncObjects.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "SyncObjects.h"


SyncObjects::SyncObjects(GraphicsDevice& graphics)
	:	device(graphics.getLogical())
{
	createSyncObjects();
}

SyncObjects::~SyncObjects()
{
	for (size_t iFrame = 0; iFrame < MaxFramesInFlight; ++iFrame) {
		vkDestroyFence(device, inFlightFences[iFrame], nullALLOC);
		vkDestroySemaphore(device, renderFinishedSemaphores[iFrame], nullALLOC);
		vkDestroySemaphore(device, imageAvailableSemaphores[iFrame], nullALLOC);
	}
}


void SyncObjects::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo = {
		.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= 0
	};

	VkFenceCreateInfo fenceInfo = {
		.sType	= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.pNext	= nullptr,
		.flags	= VK_FENCE_CREATE_SIGNALED_BIT	// Start Fence in signalled state.
	};

	for (int iFrame = 0; iFrame < MaxFramesInFlight; ++iFrame)
	{
		call = vkCreateSemaphore(device, &semaphoreInfo, nullALLOC, &imageAvailableSemaphores[iFrame]);
		if (call == VK_SUCCESS) {
			call = vkCreateSemaphore(device, &semaphoreInfo, nullALLOC, &renderFinishedSemaphores[iFrame]);
			if (call == VK_SUCCESS) {
				call = vkCreateFence(device, &fenceInfo, nullALLOC, &inFlightFences[iFrame]);
				if (call == VK_SUCCESS)
					continue;
			}
		}
		Fatal("Create synchronization object for frame " + to_string(iFrame) + " FAILURE" + ErrStr(call));
	}
}
