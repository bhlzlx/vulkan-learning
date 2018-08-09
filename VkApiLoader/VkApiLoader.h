#pragma once

#ifdef _WIN32
	#define VK_NO_PROTOTYPES
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.h"

namespace clannad
{
	namespace vulkan
	{
		struct InstanceApi
		{
#define VULKAN_INSTANCE_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#include "inl/instanceFunctions.inl"
		};

		struct DeviceApi
		{
#define VULKAN_DEVICE_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#include "inl/deviceFunctions.inl"
		};

		struct GlobalApi
		{
#define VULKAN_GLOBAL_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#define VULKAN_EXPORT_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#include "inl/exportFunctions.inl"
#include "inl/globalFunctions.inl"
		};

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
		private:
			GlobalApi __globalApi;
		public:
			ApiLoader() {
			}
			~ApiLoader() {
			}
		public:
			bool Initialize();
			bool CreateInstance(VkInstance& inst_, InstanceApi& api_);
			bool CreateDevice( const InstanceApi& _instApi, VkInstance _inst, size_t _preferedPD, DeviceLoadData& _device, DeviceApi& api_ );
		};
	}

}
