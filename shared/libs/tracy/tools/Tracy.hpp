#ifndef __TRACYLIGHT_H__
#define __TRACYLIGHT_H__

// Disable warning from tracy header
// Include "tools/Tracy.hpp" to use Tracy in your project
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324)

	// Angled brackets forces the compiler to look for the header in the include path
	// rather than itself
	#include <Tracy.hpp>

	#pragma warning(pop)
#else
	#include <Tracy.hpp>
#endif

#endif // __TRACYLIGHT_H__