//
// VulkanPlatform.h
//	Vulkan Module Setup
//
// Convenience typedefs, externs, some oft-used implementation
//	and abbreviation, used throughout Vulkan setup, trying to
//	make it a bit easier to use.  Also to help uniformly
//	tie-in the platform interface.
//	For instance, 
	//
	//TJ_TODO: I'm using this file in a more widespread fashion
	//	than just for Vulkan.  Consider renaming/relocating it
	//	to something more general.
	//
	//	...also...
	//COMMENT re: 'using std::'s can be something like:
	//	This is controversial, but
	//	...throughout this code...
	//	the key is really that we're STANDARDIZING on implementations
	//	like std::string for ALL OUR STRINGS, and that
	//	all the 'std::'s clumsily add a lot of noise to our source,
	//	(versus cleaner languages like Swift or C#), when the risk
	//	of some_other_lib::string "sneaking in" is practically
	//	non-existent (or existing only to you pedants out there).
	//
//
// StringArray is to avoid bloat of e.g. std::vector<std::string>
//	simply to ease C's inability to pass a sized constant string
//	array (since it reduces passed-in array references to simple
//	pointers, losing a preinitialized array's size information).
//
// Created 3/2/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef VulkanPlatform_h
#define VulkanPlatform_h

#include "vulkan/vulkan.h"

#include <string>
using std::string;			// To directly avoid: using namespace std;
using std::to_string;		//	but to also declutter our entire codebase, these are
							//	absolutely not expected to lurk in some other library.
#include <vector>
using std::vector;			// ...and while we're at it,
#include <stdexcept>		//	these additional ones are
using std::exception;		//	well-established as well.
using std::runtime_error;


#define N_ELEMENTS_IN_ARRAY(ELEMENTS)	sizeof(ELEMENTS) / sizeof(ELEMENTS[0])

extern VkResult call;		// return result of function call  ...for anyone to use

							// Use this for vkFunction calls passing: const VkAllocationCallbacks* pAllocator
extern const VkAllocationCallbacks* const nullALLOC;		// (and give it an explicit unmistakable TYPE too)
							//	to make that seldom-used parameter stand out from actually-significant nullptrs.

typedef const char*		StrPtr;			// Abstract away (somewhat) pointer to string used by Vulkan.
typedef uint32_t		ArrayCount;		// Abstract-out a Vulkan-typical array size indicator.
typedef unsigned int	Index;			// Abstract general means of indexing into array.


enum Tier { ERROR, WARN, NOTE, RAW };
extern void Log(Tier tier, string message);
extern void Log(Tier tier, const char* format, ...);

inline int Fatal(string message) { throw runtime_error("FATAL: " + message); }
// (This is inlined, because if inside a non-void function, an otherwise meaningless return value is required after it.)

extern string ErrStr(VkResult);

extern const char* VkFormatString(VkFormat);

/*#define noteOnError(ARG)	if (call != VK_SUCCESS)  Note(ARG);
#define dieOnFailure(ARG)	if (call != VK_SUCCESS)  Fatal(ARG);
*/
extern void DumpStringVector(StrPtr label, vector<StrPtr> strray);


/*struct StringArray // works well when the strings already exist on the heap (won't free them)
{
	//uint32_t	count;
	vector<const char*>	strings;
	//inline void setCount()	{ count = (sizeof(this) - sizeof(uint32_t)) / sizeof(const char*); };
	uint32_t count() { return (uint32_t) strings.size(); };
	const char** pStrings() { return strings.data(); };
	void append(const char* str) { strings.emplace_back(str); };
};*/
struct StringArray
{
	ArrayCount	count;
	StrPtr*		pStrings;
};


#endif // VulkanPlatform_h
