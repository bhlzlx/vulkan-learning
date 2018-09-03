#pragma once

#ifdef _WIN32
	#define VK_NO_PROTOTYPES
	#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>

#define VULKAN_EXPORT_API( api ) static PFN_##api api;
#define VULKAN_GLOBAL_API( api ) static PFN_##api api;
#define VULKAN_INSTANCE_API( api ) static PFN_##api api;
#define VULKAN_DEVICE_API( api ) static PFN_##api api;

namespace clannad
{
	namespace vulkan
	{
#include "inl/functions.h"
	}

}
