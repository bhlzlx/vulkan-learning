#pragma once
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#define USE_SWAPCHAIN_EXTENSIONS

#include "functions/functions.h"

namespace vknpp
{
	struct Dylib;
	struct Instance;
	struct PhysicalDevice;
	struct LogicalDevice;
};