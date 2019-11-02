//
// Utility.h
//	Vulkan Assist
//
// General utility functions, generally catered to Vulkan,
//	especially as related to GPU/hardware.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef Utility_h
#define Utility_h

#include "iPlatform.h"


enum UnitPrefixConvention { BinaryPrefixRAMSizes	= 1024,
							DecimalPrefixDiskSizes	= 1000 };

class Utility
{
public:
	static uint64_t GetByteSizeVRAM(VkPhysicalDevice& device);
	static string	StringBytesWithUnitPrefix(uint64_t nBytes,
											  UnitPrefixConvention kiloValue = BinaryPrefixRAMSizes);
};

#endif // Utility_h
