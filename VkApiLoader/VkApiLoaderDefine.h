#pragma once
#include <vulkan/vulkan.h>

namespace clannad
{
	namespace vulkan
	{
		static VkApplicationInfo defAppInfo =
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
			nullptr,                                        // const void                *pNext
			"clannad afterstory~",  // const char                *pApplicationName
			VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   applicationVersion
			"clannad",                     // const char                *pEngineName
			VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   engineVersion
			VK_MAKE_VERSION(1, 0, 0)                      // uint32_t                   apiVersion
		};

		static const char * defExts[] =
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
		};

		VkInstanceCreateInfo defInstCreateInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,
			&defAppInfo,
			0,
			nullptr,
			sizeof(defExts) / sizeof(const char *),
			&defExts[0]
		};

		static char PhysicalDevicePreferList[][8] = {
			"NVIDIA",
			"ATI",
			"AMD",
			"INTEL"
		};

		extern VkAllocationCallbacks* DefaultAllocCallbacks;
	}	
}
