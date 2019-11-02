//
// SDL Equivalents.h
//	Platform concretion
//
// Simple "table" equating Vulkan and SDL Pixel Formats.
//
// Note that determining support for an image format involves several steps:
//	1. First and simplest, look it up below.  It may translate straight across.
//	2. If UNSUPPORTED below, use SDL to convert into a default/common format.
//	3. Even if the image is returned (and perhaps converted to) a VK_FORMAT,
//	   the graphics hardware may still lack support.  Then SDL may still be
//	   used to convert -- as directed by the app -- to alternate formats
//	   until a supported one is found.
//
// Created 6/11/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef sdlEquivalents_h
#define sdlEquivalents_h


struct FormatEquivalent {
	SDL_PixelFormatEnum		sdlFormat;
	VkFormat				vkFormat;
};

const VkFormat UNSUPPORTED	= VK_FORMAT_UNDEFINED;


// An attempt was made to "somewhat optimize" this table by rearranging it in order of
//	formats "judged most common" first (i.e. vs. the original orderings of either
//	SDL_PIXELFORMATs or VK_FORMATs).  Especially, 32-bit formats were shifted up and
//	unsupported ones down.  A "common" format would be e.g. VK_FORMAT_R8G8B8A8_UNORM.

const FormatEquivalent	EquivalencyTable[] = {		// There's a DEV NOTE 1 (at bottom) musing some of these
													//	pixel format conversion details.  Like what UNORM is.
	{ SDL_PIXELFORMAT_UNKNOWN,		VK_FORMAT_UNDEFINED				},
	// 32-bit
	{ SDL_PIXELFORMAT_RGBA8888,		VK_FORMAT_R8G8B8A8_UNORM		},
	{ SDL_PIXELFORMAT_ABGR8888,		VK_FORMAT_A8B8G8R8_UNORM_PACK32	},
	{ SDL_PIXELFORMAT_RGBX8888,		VK_FORMAT_R8G8B8A8_UNORM		},
	{ SDL_PIXELFORMAT_BGRA8888,		VK_FORMAT_B8G8R8A8_UNORM		},
	{ SDL_PIXELFORMAT_BGRX8888,		VK_FORMAT_B8G8R8A8_UNORM		},
	{ SDL_PIXELFORMAT_ARGB8888,		VK_FORMAT_B8G8R8A8_UNORM		},	// This works despite being reversed; the key seems to be ARGB / BGRA order.
	{ SDL_PIXELFORMAT_ARGB2101010,	UNSUPPORTED						},
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	{ SDL_PIXELFORMAT_RGBA32,		VK_FORMAT_R8G8B8A8_UNORM		},
	{ SDL_PIXELFORMAT_BGRA32,		VK_FORMAT_B8G8R8A8_UNORM		},
	{ SDL_PIXELFORMAT_ABGR32,		VK_FORMAT_A8B8G8R8_UNORM_PACK32	},
	{ SDL_PIXELFORMAT_ARGB32,		UNSUPPORTED						},
#else		// Intel being little-endian (LSB first), JPEG/PNG big-endian (network order, MSB first), these ARRAYs need order interpreted as reversed (see DEV NOTE 2)
	{ SDL_PIXELFORMAT_RGBA32,		VK_FORMAT_A8B8G8R8_UNORM_PACK32 },
	{ SDL_PIXELFORMAT_ARGB32,		VK_FORMAT_B8G8R8A8_UNORM		},
	{ SDL_PIXELFORMAT_ABGR32,		VK_FORMAT_R8G8B8A8_UNORM		},
	{ SDL_PIXELFORMAT_BGRA32,		UNSUPPORTED						},
#endif
	// 24-bit
	{ SDL_PIXELFORMAT_RGB24,		VK_FORMAT_R8G8B8_UNORM			},	// Note, on Apple/AMD produces: VK_ERROR_FORMAT_NOT_SUPPORTED: VkFormat VK_FORMAT_R8G8B8_UNORM is not supported on this platform.
	{ SDL_PIXELFORMAT_BGR24,		VK_FORMAT_B8G8R8_UNORM			},	//	(See DEV NOTE 2 about these.  SDL's are 24-bit 3-byte.)
	{ SDL_PIXELFORMAT_BGR888,		VK_FORMAT_A8B8G8R8_UNORM_PACK32	},	// SDL specifically defines both	// (Here SDL puts "don't care" channel as high byte
	{ SDL_PIXELFORMAT_RGB888,		VK_FORMAT_R8G8B8A8_UNORM		},	//	these as 24-bit but 4-byte.		//	and here as low byte.)
	// 16-bit
	{ SDL_PIXELFORMAT_RGBA4444,		VK_FORMAT_R4G4B4A4_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_BGRA4444,		VK_FORMAT_B4G4R4A4_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_ARGB1555,		VK_FORMAT_A1R5G5B5_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_RGBA5551,		VK_FORMAT_R5G5B5A1_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_BGRA5551,		VK_FORMAT_B5G5R5A1_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_RGB565,		VK_FORMAT_R5G6B5_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_BGR565,		VK_FORMAT_B5G6R5_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_ARGB4444,		UNSUPPORTED						},
	{ SDL_PIXELFORMAT_ABGR4444,		UNSUPPORTED						},
	{ SDL_PIXELFORMAT_ABGR1555,		UNSUPPORTED						},
	// 12-15-bit, zero-bit-padded to 16
	{ SDL_PIXELFORMAT_RGB444,		VK_FORMAT_R4G4B4A4_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_RGB555,		VK_FORMAT_R5G5B5A1_UNORM_PACK16	},
	{ SDL_PIXELFORMAT_BGR555,		VK_FORMAT_B5G5R5A1_UNORM_PACK16	},
	// 8-bit
	{ SDL_PIXELFORMAT_RGB332,		VK_FORMAT_R8_UNORM				},
	// indexed/bitmapped color
	{ SDL_PIXELFORMAT_INDEX1LSB,	UNSUPPORTED						},
	{ SDL_PIXELFORMAT_INDEX1MSB,	UNSUPPORTED						},
	{ SDL_PIXELFORMAT_INDEX4LSB,	UNSUPPORTED						},
	{ SDL_PIXELFORMAT_INDEX4MSB,	UNSUPPORTED						},
	{ SDL_PIXELFORMAT_INDEX8,		UNSUPPORTED						},
	// Y U V
	{ SDL_PIXELFORMAT_YV12,			UNSUPPORTED						},	// Planar mode: Y + V + U  (3 planes)
	{ SDL_PIXELFORMAT_IYUV,			UNSUPPORTED						},	// Planar mode: Y + U + V  (3 planes)
	{ SDL_PIXELFORMAT_YUY2,			UNSUPPORTED						},	// Packed mode: Y0+U0+Y1+V0 (1 plane)
	{ SDL_PIXELFORMAT_UYVY,			UNSUPPORTED						},	// Packed mode: U0+Y0+V0+Y1 (1 plane)
	{ SDL_PIXELFORMAT_YVYU,			UNSUPPORTED						},	// Packed mode: Y0+V0+Y1+U0 (1 plane)
	{ SDL_PIXELFORMAT_NV12,			UNSUPPORTED						},	// Planar mode: Y + U/V interleaved  (2 planes)
	{ SDL_PIXELFORMAT_NV21,			UNSUPPORTED						},	// Planar mode: Y + V/U interleaved  (2 planes)
	//TJ_TODO: I have no idea what to do with this next one!
	{ SDL_PIXELFORMAT_EXTERNAL_OES,	UNSUPPORTED						} 	// Android video texture format
};

const int N_FORMAT_EQUIVALENCIES = N_ELEMENTS_IN_ARRAY(EquivalencyTable);


#endif	// sdlEquivalents_h


/* DEV NOTE 1 -- Regarding UNORM and type

	https://wiki.libsdl.org/SDL_PixelFormatEnum
	Although it's not perfectly clear how those match up, especially to a _UNORM as opposed to a _UINT.
	You'd think UNORM would always imply FLOAT, but perhaps not.

	Reference:
	https://android.googlesource.com/platform/external/vulkan-validation-layers/+/HEAD/layers/vk_format_utils.cpp

	According to Microsoft for D3D:
		snorm float - IEEE 32-bit signed-normalized float in range -1 to 1 inclusive.
		unorm float - IEEE 32-bit unsigned-normalized float in range 0 to 1 inclusive.
	BUT our 8-bit-per-channel formats obviously aren't using floating point in 8 bits, so are perhaps
		"fixed point" where 0 to 255 represents 1/256 steps between 0 and 1, so is "linear." <--(speculation)
	List of formats not supported on Mac/AMD, hence returning errors (or strangely locking up Xcode) :
		VK_FORMAT_B8G8R8A8_UINT, VK_FORMAT_B8G8R8_UNORM or VK_FORMAT_R8G8B8_UNORM
	Note that VK_FORMAT_B8G8R8A8_SRGB works, but looks dark (SRGB scales down the RGB range).

	Later note:  Microsoft at least provides some good documentation/explanation here, which extends to Vulkan:
		https://docs.microsoft.com/en-us/windows/desktop/api/dxgiformat/ne-dxgiformat-dxgi_format
	(see bottom of document for explanation of _UNORM and friends)

	Oh, and THIS!
		https://vulkan.lunarg.com/doc/view/1.0.30.0/linux/vkspec.chunked/ch31s03.html
	Much explanation of the formats.
*/
/* ADDENDUM
	This proved insightful in converting SDL_PIXELFORMAT_ to VK_FORMAT_ :
https://archive.fosdem.org/2018/schedule/event/pixel_formats/attachments/slides/2627/export/events/attachments/pixel_formats/slides/2627/fosdem_2018_pixel_format_guide.pdf
	Apparently there's a Python tool that helps.
	In particular, for a PNG returning SDL_PIXELFORMAT_ARGB8888, which doesn't seem to have a
	direct VK_FORMAT equivalent (no VK_FORMAT_A8R8G8B8_ defined), a valid alternative appears
	to be VK_FORMAT_B8G8R8A8_UNORM, although otherwise accommodating Endianness may be necessary.
*/
/* DEV NOTE 2 -- Regarding Endianness

	Image formats like JPEG and PNG are Big Endian (or what's called "network order") while common
	platforms like Intel CPUs are Little Endian.  This affects the ordering of bytes in a word or
	long-word as they are transferred or are written into memory -- whether the Most Significant
	Byte (MSB) or Least Significant Byte (LSB) comes first.

	In a diagram, take the following pixel color, brown, a 32-bit value = 0xAA5500FF
	Bytes are received and written into memory:

	 Big Endian or					>		  Byte		Little Endian		(Big Endian
	  MSB-first or		 Value		>		 memory		 (Intel CPU)			(?)
	"network order"		received	>		location	  platform			  platform)
	 -------------		--------	>		--------	  --------			  --------
	    byte 1			  0xAA		>			0			0xFF				0xAA
	    byte 2			  0x55		>			1			0x00				0x55
	    byte 3			  0x00		>			2			0x55				0x00
	    byte 4			  0xFF		>			3			0xAA				0xFF
	
	It all depends on how the memory is accessed.  A LONG WORD at location 0 is always = 0xAA5500FF.
	However, on Intel CPUs, the BYTE at location 0 is 0xFF, opposite the order it was received.
	If an Intel CPU writes the long word into memory, this "reversal" when written is automatic.
	Although if the bytes are written one-by-one, they must be explicitly reversed on Intel, and
	WORD SIZE is of critical importance (e.g. 16-bit word, 32-bit long word, 64-bit long long word).
	Still though, the Intel CPU usually takes care of these details for us.

	Pixel formats like the common 32-bit RGBA, while a long word, require us to think about
	bytes and their orderings, to interpret each of the single-byte R, G, B, and A channels.
	Using a library like SDL_image, the question is whether the library does this for us and how
	it, itself, may interpret endianness.

	https://wiki.libsdl.org/SDL_PixelFormatEnum  offers some explanation.

	Formats like SDL_PIXELFORMAT_RGBA8888 imply interpretation as a 32-bit long word quantity, so
	Red is in the high byte and Alpha is in the low byte.
	Alternately, SDL_PIXELFORMAT_RGBA32 is interpreted as an ARRAY OF 4 BYTES, so is subject to
	endianness.  Hence the "#if SDL_BYTEORDER == SDL_BIG_ENDIAN" above and in: SDL_pixels.h

	SPECIAL NOTE on SDL_PIXELFORMAT_RGB24 and SDL_PIXELFORMAT_BGR24 formats:
	While you'd think these would be treated the same way (with a similar #if (*)), these
	24-bit/3-byte formats "almost always" report as UNSUPPORTED on modern GPUs that optimize or
	force operation on word (or larger) boundaries.  So Vulkan typically rejects them, requiring
	us to again call upon the library to SDL_ConvertSurfaceFormat().

	(*) -- note that in SDL_pixels.h, these 24-bit PIXELFORMATS are indeed DEFINEd as "ARRAY"
	and not "PACKED" whereas the RGBA32/ARGB32 formats alias to actual PACKED formats, based on
	endianness.  Thus RGBA24/ARGB24 probably don't deserve the same #if treatment.

	So again, for 32-bits, PACKED interprets as long word, versus ARRAY as four bytes.
	Consider this with _PACK32 in the EquivalencyTable.
*/
