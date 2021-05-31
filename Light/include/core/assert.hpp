#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "core/base.hpp"
#include "core/logging.hpp"

#ifndef NDEBUG
	#define _LIGHT_ASSERT_IMPL(type, cond, msg, ...)	{if(!(cond)) { LIGHT##type##ERROR(msg, __VA_ARGS__);\
																		LIGHT_DEBUG_BREAK(); }}
	#define _LIGHT_ASSERT_2(type, cond, ...)	_LIGHT_ASSERT_IMPL(type, cond, "Assertion '{0}' failed: {1}",\
																	_LIGHT_STRINGIFY_MACRO(cond), __VA_ARGS__)
	#define _LIGHT_ASSERT_1(type, cond)			_LIGHT_ASSERT_IMPL(type, cond, "Assertion '{0}' failed",\
																	_LIGHT_STRINGIFY_MACRO(cond))

	#define _LIGHT_GET_3RD_ARG(arg1, arg2, arg3, ...)	arg3
	#define _LIGHT_GET_ASSERT_MACRO(...)				_LIGHT_EXPAND_MACRO(_LIGHT_GET_3RD_ARG(__VA_ARGS__, _LIGHT_ASSERT_2, _LIGHT_ASSERT_1))

	#define LIGHT_CORE_ASSERT(...)	_LIGHT_EXPAND_MACRO(_LIGHT_GET_ASSERT_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
	#define LIGHT_ASSERT(...)		_LIGHT_EXPAND_MACRO(_LIGHT_GET_ASSERT_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#else
	#define LIGHT_CORE_ASSERT(...)
	#define LIGHT_ASSERT(...)
#endif

#endif // __ASSERT_H__