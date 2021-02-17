#pragma once
// DLL import/export macro definitions for easier code reading

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
	#error Hazel only supports Windows!
#endif // HZ_PLATFORM_WINDOWS

// Bit shift by x
#define BIT(x) (1 << x)