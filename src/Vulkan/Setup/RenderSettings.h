//
// RenderSettings.h
//	Vulkan Setup
//
// Render-related variables, encapsulated.
//
// 7/15/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef RenderSettings_h
#define RenderSettings_h

struct
{
	const VkBool32	useAnisotropy	= false;	// (disable for performance)
	const float		anisotropyLevel	= 16;		//	(or decrease this value) (values > 16 superfluous)

} RenderSettings;

#endif	// RenderSettings_h
