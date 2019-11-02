//
// Utility.cpp
//	Vulkan Assist
//
// See matched header file for definitive comment.
//
// 2/27/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "Utility.h"


// Thanks to Sascha Willems https://stackoverflow.com/questions/44339931/query-amount-of-vram-or-gpu-clock-speed
//
// TJ: NOTE that on MacOS, until I can test this on other platforms, the previously-coded
//	and recommended-by-the-above-link approach -- which is: bitwise-ANDing heap.flags with
//	VK_MEMORY_HEAP_DEVICE_LOCAL_BIT (and only seems natural, given ..._BIT) -- doesn't return
//	consistent results.  Try a #define DEBUG_LOW to see for yourself, and you may witness --
//	a first value that looks correct and has flags 0x1 -- followed by, randomly, any number
//	of heap.size values and heap.flags of huge values also having their low bit set as well.
//	Therefore, now we specifically look for heap.flags of that 1-bit and that 1-bit only.
//
uint64_t Utility::GetByteSizeVRAM(VkPhysicalDevice& device)
{
	//#define DEBUG_LOW
	uint64_t totalVRAM = 0;
	VkPhysicalDeviceMemoryProperties	memoryProperties;

	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

	for (const auto& heap : memoryProperties.memoryHeaps)
		if (heap.flags == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)				//TJ: FOR MAC, instead of this:
		//if ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0)		//TJ: (which seems more correct)
		{
			totalVRAM += heap.size;
			#ifdef DEBUG_LOW
				printf("  heap.size: %llu  .flags: 0x%X\n", heap.size, heap.flags);
			#endif
		}
		#ifdef DEBUG_LOW
			printf("  TOTAL VRAM: %llu\n", totalVRAM);
		#endif
	return totalVRAM;
}

string Utility::StringBytesWithUnitPrefix(uint64_t nBytes, UnitPrefixConvention kiloValue)
{
	const double kilo = kiloValue;
	const double mega = kilo * kilo;
	const double giga = kilo * kilo * kilo;

	double fBytes = double(nBytes);
	char preCh = '\0';
	int nDigits = 0;

	if (nBytes >= giga) {			// e.g. 12.34 G
		fBytes /= giga;		preCh = 'G';	nDigits = 2;
	} else if (nBytes >= mega) {	// e.g. 345.6 M
		fBytes /= mega;		preCh = 'M';	nDigits = 1;
	} else if (nBytes >= kilo) {	// e.g.   678 K
		fBytes /= kilo;		preCh = 'K';	nDigits = 0;
	}
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%.*lf %c", nDigits, fBytes, preCh);

	return buffer;
}
