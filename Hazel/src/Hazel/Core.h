#pragma once
// DLL import/export macro definitions for easier code reading

// On windows set dll export/import macros
#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
	#define HAZEL_API
#endif
#else
	#error Hazel only supports Windows!
#endif // HZ_PLATFORM_WINDOWS

// If we are in debug mode, enable asserts
#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif // HZ_DEBUG

// Assert macros
#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif // HZ_ENABLE_ASSERTS

// Bit shift by x
#define BIT(x) (1 << x)

// Binding funtions to event types for event dispatching
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
