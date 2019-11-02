//
// Helpers.h
//
// Simple inline convenience functions.
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef Helpers_h
#define Helpers_h


// Clamp a given 1st reference to within the minimum and maximum, inclusive.
//	This inline specifically operates on unsigned values, which don't have
//	to be checked for invalid values like min == 0 or max == max_int.
//	If the min/max range is erroneous, does nothing.
//
inline void clamp(uint32_t& value, const uint32_t min, const uint32_t max)
{
	if (min > max)
		return;

	if (value < min)	// && min > 0 // these are implied
		value = min;
	if (value > max)	// && max < numeric_limits<uint32_t>::max()
		value = max;
}

#endif // Helpers_h
