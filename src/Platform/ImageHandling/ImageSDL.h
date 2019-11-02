//
// ImageSDL.h
//	General App Chassis
//
//	SDL concretion of iImageSource.
//
// Created 2/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ImageSDL_h
#define ImageSDL_h

#include "iImageSource.h"

#include <SDL_image.h>

#include "sdlEquivalents.h"


class ImageSDL : public iImageSource
{
	const SDL_PixelFormatEnum	ARGUABLY_MOST_COMMON_SDL_FORMAT = SDL_PIXELFORMAT_ARGB8888;

	const VkFormat				ARGUABLY_MOST_GENERAL_VK_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;

private:
	SDL_Surface*	pImage;

public:
	ImageSDL() : pImage(nullptr)  { }

	~ImageSDL()
	{
		if (pImage)
			SDL_FreeSurface(pImage);
	}

protected:
	// On exit, sets: pixels, size, format
	//
	ImageInfo Load(StrPtr filePath)
	{
		Log(RAW, "Load: %s", filePath);

		pImage = IMG_Load(filePath);

		if (!pImage || pImage->pixels == NULL)
			Fatal("SDL failed to IMG Load texture image: " + string(filePath));

		return validatedImageInfo();
	}

	ImageInfo validatedImageInfo(const char* strOp = "")
	{
		void* pPixels	= pImage->pixels;
		int   texWidth	= pImage->w;
		int   texHeight	= pImage->h;
		VkDeviceSize imageSize = texHeight * pImage->pitch;

		Uint32 sdlPixelFormat = pImage->format->format;
		int bitsPerPixel  = SDL_BITSPERPIXEL(sdlPixelFormat);
		int formatBytesPerPixel = SDL_BYTESPERPIXEL(sdlPixelFormat);
		Log(RAW, "      SDL Image Pixel Format%s: %s  (%d-bit, %d bytes per pixel)", strOp,
					SDL_GetPixelFormatName(sdlPixelFormat), bitsPerPixel, formatBytesPerPixel);
		int computedBytesPerPixel = pImage->format->BytesPerPixel;

		VkFormat vkFormat = findVkFormatEquivalentTo((SDL_PixelFormatEnum) sdlPixelFormat);

		if (vkFormat == VK_FORMAT_UNDEFINED)
		{
			Log(WARN, "Unable to match %s to a VK_FORMAT_ equivalent.", SDL_GetPixelFormatName(sdlPixelFormat));
			vkFormat = ARGUABLY_MOST_GENERAL_VK_FORMAT;
			Log(WARN, "    Using fallback 'general' VkFormat: %s", VkFormatString(vkFormat));
			Log(NOTE, "    If Image Coloring is wrong, add or adjust the Format in ImageSDL's Equivalency Table.");
		} else {
			// SDL format's bits/bytes may not agree (e.g. RGB888 is 24-bit but 4-byte).  While the EquivalencyTable
			//	should translate those correctly (e.g. treating RGB888 as R8G8B8A8), still, output a warning.
			if (bitsPerPixel / 8 != computedBytesPerPixel || formatBytesPerPixel != computedBytesPerPixel)
				Log(RAW, "   Warning: SDL indicates a %d-bit but %d-byte pixel format (or separately BytesPerPixel = %d)",
					bitsPerPixel, formatBytesPerPixel, computedBytesPerPixel);
			Log(RAW, "      Vulkan equivalent VkFormat: %s", VkFormatString(vkFormat));
		}

		return { pPixels, imageSize, vkFormat, texWidth, texHeight };
	}

	VkFormat findVkFormatEquivalentTo(SDL_PixelFormatEnum sdlFormatRequested)
	{
		for (int iEquivalent = 0; iEquivalent < N_FORMAT_EQUIVALENCIES; ++iEquivalent)
			if (EquivalencyTable[iEquivalent].sdlFormat == sdlFormatRequested)
				return EquivalencyTable[iEquivalent].vkFormat;
		return UNSUPPORTED;
	}

	SDL_PixelFormatEnum findSDLFormatEquivalentTo(VkFormat vkFormatRequested)
	{
		for (int iEquivalent = 0; iEquivalent < N_FORMAT_EQUIVALENCIES; ++iEquivalent)
			if (EquivalencyTable[iEquivalent].vkFormat == vkFormatRequested)
				return EquivalencyTable[iEquivalent].sdlFormat;
		return SDL_PIXELFORMAT_UNKNOWN;
	}

	ImageInfo ConvertTo(VkFormat toFormat, bool fallbackOnFailure = true)
	{
		SDL_PixelFormatEnum equivalentSDLFormat = findSDLFormatEquivalentTo(toFormat);

		if (equivalentSDLFormat == SDL_PIXELFORMAT_UNKNOWN && fallbackOnFailure) {
			Log(WARN, "Cannot find SDL Format equivalent to %s, so CONVERTing to %s", VkFormatString(toFormat),
													  SDL_GetPixelFormatName(ARGUABLY_MOST_COMMON_SDL_FORMAT));
			equivalentSDLFormat = ARGUABLY_MOST_COMMON_SDL_FORMAT;
		}

		SDL_Surface* pNewImage = SDL_ConvertSurfaceFormat(pImage, equivalentSDLFormat, 0);

		if (!pNewImage || pNewImage->pixels == NULL)
			Fatal("Failed to SDL Convert Surface Format from " + string(SDL_GetPixelFormatName(pImage->format->format))
													  + " to " + string(SDL_GetPixelFormatName(equivalentSDLFormat)));

		if (pImage)						// succeeded, so free previous
			SDL_FreeSurface(pImage);
		pImage = pNewImage;				// and replace it with newly converted

		return validatedImageInfo(" CONVERTED to");
	}
};

#endif	// ImageSDL_h
