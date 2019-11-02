//
// iImageSource.h
//	Platform-related "Interface"/Base Class
//	General App Chassis		Vulkan-centric
//
// Image Storage Handler/Loader ABSTRACTION.
//	Platform-specific image loaders adhere to this interface,
//	especially providing determination of image type, size,
//	pixel format, and knowing how to decompress/decode into
//	a generic pixel buffer and return Vulkan-relevant format.
// Another encapsulated detail: platform-specific path to
//	image/texture, like when packaged into an app's "bundle."
//
// Created 3/4/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef iImageSource_h
#define iImageSource_h

#include "VulkanPlatform.h"


struct ImageInfo {

	void*	pPixels;

	VkDeviceSize numBytes;

	VkFormat	 format;

	int		wide,
			high;
};

class iImageSource
{
		// pure-virtual methods
public:
	virtual ImageInfo Load(StrPtr fileName) = 0;

	virtual ImageInfo ConvertTo(VkFormat format, bool fallbackOnFailure = true) = 0;

		// publicly-readable members (for simplicity)

};

#endif // iImageSource_h
