#pragma once

#ifdef _WIN32
	#define VK_NO_PROTOTYPES
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.h"
#include "vkApi.h"

namespace clannad
{
	namespace vulkan
	{
		struct DeviceLoadData
		{
			VkDevice _id;
			VkPhysicalDevice _host;
			uint32_t _graphicQueueFamily;
		};

		//
		class ApiLoader
		{
#ifdef _WIN32
			HMODULE __vulkan_1_dll;
#endif
		public:
			ApiLoader() {
			}
			~ApiLoader() {
			}
		public:
			bool Initialize();
			VkInstance CreateInstance();
			bool CreateDevice(const VkInstance& _inst, size_t _preferedPD, DeviceLoadData& _device);
		};
	}

}
