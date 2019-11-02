//
// ValidationLayers.h
//	Vulkan Setup
//
// For both Instance Layers and Validation Layers.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ValidationLayers_h
#define ValidationLayers_h

#include "VulkanPlatform.h"
#include "PlatformSpecifics.h"	// (for __emplace_back on iOS, possibly)
#include <vector>
#include <cstdint>				// for uint32_t et al


class ValidationLayers
{		// XSTRUCTORS
public:
	ValidationLayers();

		// MEMBERS
private:
	#ifdef NDEBUG
		const bool isEnabledValidationLayers = false;
	#else
		const bool isEnabledValidationLayers = true;
	#endif

	vector<StrPtr>	availableLayerNames;

		// METHODS
private:
	void getVulkanInstanceLayers();
	void displayLayerNames(VkLayerProperties* pLayerProperties, int nProperties);
public:
	uint32_t	NumEnabledLayers();
	const char* const* ppEnabledLayerNames();
	bool		IsValidationProvided();
};

#endif // ValidationLayers_h
