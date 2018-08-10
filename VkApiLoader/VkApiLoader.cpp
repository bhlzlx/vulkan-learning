#include "VkApiLoader.h"
#include "VkApiLoaderDefine.h"
#include <vector>
#include <iostream>

namespace clannad
{
	namespace vulkan
	{
		VkAllocationCallbacks* DefaultAllocCallbacks = nullptr;
		//

		bool ApiLoader::Initialize()
		{
#ifdef _WIN32
			__vulkan_1_dll = ::LoadLibrary(L"vulkan-1.dll");
			if (!__vulkan_1_dll)
				return false;
			__globalApi.vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(__vulkan_1_dll, "vkGetInstanceProcAddr");
			if (!__globalApi.vkGetInstanceProcAddr)
				return false;
			#define VULKAN_GLOBAL_API(api)\
				if (!(__globalApi.##api = (PFN_##api)__globalApi.vkGetInstanceProcAddr(nullptr, #api))) {\
				\
				  std::cout << "Could not load global level function: " << #api << "!" << std::endl;  \
				  return false;                                                                       \
				}
			#include "inl/globalFunctions.inl"
						return true;
#endif
		}

		bool ApiLoader::CreateInstance( Instance& inst_ )
		{
			VkInstance& instId = inst_._id;
			InstanceApi& api = inst_.api;
			//
			uint32_t nExt = 0;
			if (
				( __globalApi.vkEnumerateInstanceExtensionProperties(nullptr, &nExt, nullptr) != VK_SUCCESS) ||
				(nExt == 0)
				)
			{
				//std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
				return false;
			}

			std::vector<VkExtensionProperties> vecExt(nExt);
			if (__globalApi.vkEnumerateInstanceExtensionProperties(nullptr, &nExt, vecExt.data()) != VK_SUCCESS)
			{
				//std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
				return false;
			}
			//
			printf("Vulkan Extentions:\n");
			for (auto& ext : vecExt)
			{
				printf("   %s : %u\n", ext.extensionName, ext.specVersion);
			}
			//
			VkResult rst = __globalApi.vkCreateInstance(&defInstCreateInfo, DefaultAllocCallbacks, &inst_._id);
			if (rst != VK_SUCCESS)
			{
				return false;
			}
			//
			#define VULKAN_INSTANCE_API(api)\
				if (!(inst_.##api = (PFN_##api)__globalApi.vkGetInstanceProcAddr( inst_ , #api)))\
				{\
				  std::cout << "Could not load global level function: " << #api << "!" << std::endl;\
				  return false;\
				}
			#include "inl/instanceFunctions.inl"
			return true;
		}

		bool ApiLoader::CreateDevice(const Instance& _inst, size_t _preferedPD, DeviceLoadData& _device)
		{
			uint32_t nDevice = 0;
			VkResult rst;
			std::vector<VkPhysicalDevice>  phyDevices;
			uint32_t availGraphicQueueFamily = -1;
			rst = _inst.vkEnumeratePhysicalDevices( _inst, &nDevice, nullptr);
			if (rst != VK_SUCCESS || nDevice == 0)
			{
				return false;
			}
			phyDevices.resize(nDevice);
			rst = _inst.vkEnumeratePhysicalDevices(_inst, &nDevice, &phyDevices[0]);
			//
			printf("\n ###### List of physical devices : ######\n");
			for (auto& phyDevice : phyDevices)
			{
				VkPhysicalDeviceProperties props;
				_inst.vkGetPhysicalDeviceProperties(phyDevice, &props);
				printf("    #  %s", props.deviceName);
			}
			//
			if (_preferedPD >= phyDevices.size())
			{
				printf("Preferred device %zd is unavailable! set to default 0!", _preferedPD);
				_preferedPD = 0;
			}
			//
			VkPhysicalDevice physicalDevice = phyDevices[_preferedPD];
			//
			VkPhysicalDeviceFeatures   features;
			VkPhysicalDeviceProperties props;
			_inst.vkGetPhysicalDeviceFeatures(physicalDevice, &features);
			_inst.vkGetPhysicalDeviceProperties(physicalDevice, &props);
			uint32_t major_version = VK_VERSION_MAJOR(props.apiVersion);
			uint32_t minor_version = VK_VERSION_MINOR(props.apiVersion);
			uint32_t patch_version = VK_VERSION_PATCH(props.apiVersion);
			// simple function support test
			if ((major_version < 1) || (props.limits.maxImageDimension2D < 4096))
			{
				return false;
			}
			// retrieve queue family count
			uint32_t queueFamilyCount = 0;
			_inst.vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, nullptr);
			if (queueFamilyCount == 0)
			{
				return false;
			}
			// select an available queue family
			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			_inst.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
			uint32_t index = 0;
			for (const VkQueueFamilyProperties& property : queueFamilyProperties)
			{
				VkBool32 presentSupport = false;
				if (property.queueCount > 0 && property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					availGraphicQueueFamily = index;
					break;
				}
				++index;
			}
			//
			_device._graphicQueueFamily = availGraphicQueueFamily;
			_device._host = physicalDevice;
			// create logical device
			std::vector<float> queuePriorities = { 1.0f };
			VkDeviceQueueCreateInfo queueCreateInfo = {
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
				nullptr,                                        // const void                  *pNext
				0,                                              // VkDeviceQueueCreateFlags     flags
				_device._graphicQueueFamily,                    // uint32_t                     queueFamilyIndex
				static_cast<uint32_t>(queuePriorities.size()), // uint32_t                     queueCount
				queuePriorities.data()                         // const float                 *pQueuePriorities
			};
			VkDeviceCreateInfo deviceCreateInfo = {
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           // VkStructureType                    sType
				nullptr,                                        // const void                        *pNext
				0,                                              // VkDeviceCreateFlags                flags
				1,                                              // uint32_t                           queueCreateInfoCount
				&queueCreateInfo,                             // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
				0,                                              // uint32_t                           enabledLayerCount
				nullptr,                                        // const char * const                *ppEnabledLayerNames
				0,                                              // uint32_t                           enabledExtensionCount
				nullptr,                                        // const char * const                *ppEnabledExtensionNames
				nullptr                                         // const VkPhysicalDeviceFeatures    *pEnabledFeatures
			};
			//
			rst = _inst.vkCreateDevice( _device._host, &deviceCreateInfo, nullptr, &_device._id);
			//
			if (rst != VK_SUCCESS)
				return false;
			// Load device level api
#define VULKAN_DEVICE_API(api)\
	if (!(_device._api.##api = (PFN_##api)_inst.vkGetDeviceProcAddr(_device._id, #api))) {\
	\
      std::cout << "Could not load global level function: " << #api << "!" << std::endl;  \
      return false;                                                                       \
    }
#include "inl/deviceFunctions.inl"
			return true;
		}
	}

	

}



