//
// ImageSTB.h
//	General App Chassis
//
//	STB concretion of iImageSource.
//
// Created 2/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ImageSTB_h
#define ImageSTB_h

#include "iImageSource.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


class ImageSTB : public iImageSource
{
private:
	stbi_uc* pPixels;

public:
	~ImageSTB()
	{
		stbi_image_free(pPixels);
	}

protected:
	// On exit, sets: pixels, size, format
	//
	ImageInfo Load(StrPtr fileName)
	{
		int texWidth, texHeight, texChannels;

		pPixels = stbi_load(fileName, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = texWidth * texHeight * 4; // <--+
													//	^	==	 |
		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM; // <---------+

		ImageInfo info = { pPixels, imageSize, format, texWidth, texHeight };

		return info;
	}

	bool ConvertTo(VkFormat format, bool fallbackOnFailure = true)
	{
		// Empty: not necessary because STBI_rgb_alpha above should assure a supported usable format.
	}
};

#endif // ImageSTB_h
