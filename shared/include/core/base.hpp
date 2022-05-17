#ifndef __BASE_H__
#define __BASE_H__

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <set>

#include <iostream>
#include <fstream>
#include <sstream>


#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

#define _LIGHT_STRINGIFY_IMPL(x) #x
#define _LIGHT_STRINGIFY_MACRO(x) _LIGHT_STRINGIFY_IMPL(x)

#define _LIGHT_EXPAND_MACRO(x) x

#ifndef NDEBUG
	#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
		#define LIGHT_DEBUG_BREAK() __debugbreak()
	#elif defined(__x86_64__)
		#define LIGHT_DEBUG_BREAK() __asm__ __volatile__("int3")
	#else
		#include <cstdlib>
		#define LIGHT_DEBUG_BREAK() exit(1)
	#endif
#else
	#define LIGHT_DEBUG_BREAK()
#endif

#ifndef NDEBUG
	#if (defined(__GNUC__) && __GNUC__ >= 9) || (defined(_MSC_VER) && _MSC_VER >= 1920)
		#include <filesystem>
		namespace fs = std::filesystem;

		extern fs::path g_filesystemcpp;

		#define _LIGHT_FILE() fs::relative(fs::path(__FILE__),\
				g_filesystemcpp.parent_path().parent_path().parent_path().parent_path()).string()
	#else
		#include <string>
		#define _LIGHT_FILE() std::string(__FILE__)
	#endif
#else

#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

#endif // __BASE_H__
