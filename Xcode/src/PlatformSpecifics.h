//
// PlatformSpecifics.h
//	APPLE-CENTRIC
//
// Only make this file part of Xcode projects.
//
// (Other versions should match, such as: VisualStudio/src/PlatformSpecifics.h)
//
// Created 6/14/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef PlatformSpecifics_h
#define PlatformSpecifics_h


#include <TargetConditionals.h>	// for TARGET_OS_IPHONE


#if TARGET_OS_IPHONE	// Why Apple still has emplace_back as "experimental," lord knows.
						//	Also, they don't support it having non-default constructors, so this:

	#define emplace_back1(p1)		__emplace_back(p1)

									// Yes, this one is specific to one specific class: Described
	#define emplace_back2(p1, p2)	push_back(Described(p1, p2))


// Simulate "MACRO OVERLOADING" of emplace_back( with either 1 or 2 parameters:

	#define GET_MACRO(_1, _2, NAME,...)	  NAME

	#define emplace_back(...)	GET_MACRO(__VA_ARGS__, emplace_back2, emplace_back1)(__VA_ARGS__)


#endif


#endif // PlatformSpecifics_h
