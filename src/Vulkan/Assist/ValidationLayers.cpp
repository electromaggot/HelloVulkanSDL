//
// ValidationLayers.cpp
//	Vulkan Setup
//
// See matched header file for definitive main comment.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "ValidationLayers.h"
#include "VulkanConfigure.h"


ValidationLayers::ValidationLayers()
{
	getVulkanInstanceLayers();
}


// Grow an array of Instance Layer Names (pointers to their strings).
//
void ValidationLayers::getVulkanInstanceLayers()
{
	uint32_t nLayerProperties = 0;	// get number of layers:
	call = vkEnumerateInstanceLayerProperties(&nLayerProperties, nullptr);
	if (call != VK_SUCCESS || nLayerProperties == 0)
		Fatal("Enumerate Instance nLayerProperties " + to_string(nLayerProperties) + ErrStr(call));

	VkLayerProperties instanceLayerProperties[nLayerProperties];  // get data themselves:
	call = vkEnumerateInstanceLayerProperties(&nLayerProperties, instanceLayerProperties);
	if (call != VK_SUCCESS)
		Fatal("Enumerate Instance Layer Properties (names) FAILURE" + ErrStr(call));

	displayLayerNames(instanceLayerProperties, nLayerProperties);

	// Search according to INSTANCE_ array first; if exhausted, can stop short of searching
	//	all returned properties; also allows better report of any unfulfillable INSTANCE_.
	//  (Doesn't work as well for printing all enumerated properties, however!)

	availableLayerNames.clear();
	for (int iLayer = 0; iLayer < N_INSTANCE_LAYER_NAMES; ++iLayer) {
		auto wantLayerName = INSTANCE_LAYER_NAMES[iLayer];
		bool unsupported = true;
		for (const auto& property : instanceLayerProperties)
			if (strcmp(wantLayerName, property.layerName) == 0) {
				availableLayerNames.emplace_back(wantLayerName);
				unsupported = false;
				break;
			}
		if (unsupported) {
			if (REQUIRE_INSTANCE_LAYER[iLayer])
				Fatal("Required Instance Layer NOT Supported: " + string(wantLayerName));
			else
				Log(WARN, "Requested Layer NOT Supported: " + string(wantLayerName));
		}
	}
}

// Display layer names, highlighting those requested.  Search according to
//	Properties array first, which works best for printing them all out.
//
void ValidationLayers::displayLayerNames(VkLayerProperties* pLayerProperties, int nProperties)
{
	Log(NOTE, "Vulkan reports INSTANCE LAYERS: %d", nProperties);

	for (; nProperties; ++pLayerProperties, --nProperties) {
		auto haveLayerName = pLayerProperties->layerName;
		StrPtr match = "";
		for (const auto& wantLayerName : INSTANCE_LAYER_NAMES)
			if (strcmp(haveLayerName, wantLayerName) == 0) {
				match = " (requested)";
				break;
			}
		Log(NOTE, " %2lu. %s: %s%s", nProperties, haveLayerName, pLayerProperties->description, match);
	}
	DumpStringVector("Applying Layer", availableLayerNames);
}


uint32_t ValidationLayers::NumEnabledLayers()
{
	return isEnabledValidationLayers ? static_cast<uint32_t>(availableLayerNames.size()) : 0;
}

const char* const* ValidationLayers::ppEnabledLayerNames()
{
	return isEnabledValidationLayers ? availableLayerNames.data() : nullptr;
}


bool ValidationLayers::IsValidationProvided() {

	if (isEnabledValidationLayers)
		for (const auto& layerName : availableLayerNames)
			if (strcmp(layerName, STANDARD_VALIDATION_LAYER) == 0)
				return true;
	return false;
}
