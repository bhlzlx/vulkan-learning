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

		class Instance
		{
		public:
			VkInstance _id = nullptr;
			union
			{
				InstanceApi api;
				struct {
#define VULKAN_INSTANCE_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#include "inl/instanceFunctions.inl"
				};
			};

			operator VkInstance() const
			{
				return _id;
			}
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
			DeviceApi _api;
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
			bool CreateInstance(Instance & inst_);
			bool CreateDevice(const Instance& _inst, size_t _preferedPD, DeviceLoadData& _device);
		};
	}

}
