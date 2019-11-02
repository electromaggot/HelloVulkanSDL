//
// PlatformCommon.cpp
//	General App Chassis
//
// Implementations for convenience (global functions).
//
// These are actually intended to (eventually) be overridden/
//	overwritten with platform-specific reimplementations.
//
// Created 2/4/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanPlatform.h"		// includes StrPtr etc.		//TJ_TODO: may be sloppy referring back into Vulkan
#include <stdarg.h>

#include <iostream>
using std::cout; using std::endl;
#include <vector>


// Assumes..: enum Tier { ERROR, WARN, NOTE };
const char* Prefix[] = { "ERROR! ", "Warning: ", "Note: ", "" };

void Log(Tier tier, string message) {
	cout << Prefix[tier] << message << endl;
}
void Log(Tier tier, StrPtr format, ...)
{
	char buffer[1024];
	va_list vargs;
	va_start(vargs, format);
	vsnprintf(buffer, sizeof buffer, format, vargs);
	va_end(vargs);
	cout << Prefix[tier] << buffer << endl;
}

//--------------------------------------------------------

// Originally, apparently, the Vulkan SDK's "vulkan.hpp" file provided
//	a to_string override for VkResult.  However, it seemed to stop working.
//	Therefore, return to this:		(until I can grok some final resolution)
extern const char* VkErrorString(VkResult);

string ErrStr(VkResult num)
{
	//return to_string(num);
	return " result: " + string(VkErrorString(num));
}

//--------------------------------------------------------
//TJ_TODO: Refer to this extern in VulkanPlatform.h and reconcile comment.
const VkAllocationCallbacks* const nullALLOC = nullptr;
//--------------------------------------------------------


void DumpStringVector(StrPtr label, vector<StrPtr> strray)
{
	size_t nStrings = strray.size();
	if (nStrings < 1) return;
	cout << label << (nStrings > 1 ? "s" : "") << ": ";
	StrPtr comma = "";
	for (const auto& str : strray) {
		cout << comma << str;
		comma = ", ";
	}
	cout << endl;
}



/* DEVELOPER NOTE - design decision
One problem with the above Note(...) is that it uses the old style of variadic function parameter
 handling, for which I have encountered support to be deprecated by certain C++1X compilers.
 The following was an attempted replacement (placed in the header file) which generally worked:

template<typename... Args>
__attribute__((__format__ (__printf__, 1, 2)))
void Note(const char* format, Args... args)
{
	size_t size = snprintf(nullptr, 0, format, args...) + 1;	// extra space for '\0'
	char buffer[size];
	snprintf(buffer, size, format, args...);		// also did try: forward<Args>(args)...
	Debug(buffer);
}

 HOWEVER, without the __attribute__format__ line, the compiler complained twice with this warning:
 	Format string is not a string literal (potentially insecure)
 Yet WITH that line, then the following error:
 	Format attribute requires variadic function
 RESULT: __attribute__format__ does not seem to recognize template-style "Args... args" - just
 older-style "..." - so it's useless to silence the warning.  So this approach is held off until
 __attribute__format__ is hopefully updated/supported in all future compilers.
*/
