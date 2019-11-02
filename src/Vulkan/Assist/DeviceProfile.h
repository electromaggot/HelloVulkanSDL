//
// DeviceProfile.h
//	Vulkan Device Assist
//
// Encapsulate evaluation of Vulkan-supportive Devices.
//
// An array of these instantiates in the construction of
//	DeviceAssessment, later released in its destructor
//	(in the spirit of, or mimicking, RAII).
// "ProfilesReference" type assists with that.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DeviceProfile_h
#define DeviceProfile_h

#include "iPlatform.h"


typedef uint64_t  RawScore;


const VkFormat			VkFormat_UNSET			= (VkFormat) -1;
const VkColorSpaceKHR	VkColorSpaceKHR_UNSET	= (VkColorSpaceKHR) -1;
const VkPresentModeKHR	VkPresentModeKHR_UNSET	= (VkPresentModeKHR) -1;


struct DeviceProfile
{
	vector<StrPtr>	extensionNames;
	bool			lacksRequiredExtensions;
	VkPhysicalDeviceProperties	properties;
	VkSurfaceFormatKHR			selectedSurfaceFormat;
	VkPresentModeKHR			selectedPresentMode;
	string			description;
	// scoring:
	uint64_t		totalScore;
	RawScore		rawPerPassScore;
	RawScore		surfaceSupportScore;

	DeviceProfile() {
		memset(this, 0, sizeof(DeviceProfile));
		lacksRequiredExtensions = false;
		selectedSurfaceFormat.format = VkFormat_UNSET;
		selectedSurfaceFormat.colorSpace = VkColorSpaceKHR_UNSET;
		selectedPresentMode = VkPresentModeKHR_UNSET;
	}
};

typedef DeviceProfile (&ProfilesReference)[];


#endif // DeviceProfile_h
