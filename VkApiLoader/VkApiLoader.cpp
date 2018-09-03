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
			vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(__vulkan_1_dll, "vkGetInstanceProcAddr");
			if (!vkGetInstanceProcAddr)
				return false;
			#define VULKAN_GLOBAL_API(api)\
				if (!(api = (PFN_##api)vkGetInstanceProcAddr(nullptr, #api))) {\
				\
				  std::cout << "Could not load global level function: " << #api << "!" << std::endl;  \
				  return false;                                                                       \
				}
			#include "inl/globalFunctions.inl"
			return true;
#endif
		}

		VkInstance ApiLoader::CreateInstance()
		{
			//
			uint32_t nExt = 0;
			if (
				( vkEnumerateInstanceExtensionProperties(nullptr, &nExt, nullptr) != VK_SUCCESS) ||
				(nExt == 0)
				)
			{
				//std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
				return false;
			}

			std::vector<VkExtensionProperties> vecExt(nExt);
			if (vkEnumerateInstanceExtensionProperties(nullptr, &nExt, vecExt.data()) != VK_SUCCESS)
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
			VkInstance inst;
			VkResult rst = vkCreateInstance(&defInstCreateInfo, DefaultAllocCallbacks, &inst);
			if (rst != VK_SUCCESS)
			{
				return false;
			}
			//
			#define VULKAN_INSTANCE_API(api)\
				if (!(api = (PFN_##api)vkGetInstanceProcAddr( inst , #api)))\
				{\
				  std::cout << "Could not load global level function: " << #api << "!" << std::endl;\
				  return nullptr;\
				}
			#include "inl/instanceFunctions.inl"
			return inst;
		}

		bool ApiLoader::CreateDevice(const VkInstance& _inst, size_t _preferedPD, DeviceLoadData& device_ )
		{
			uint32_t nDevice = 0;
			VkResult rst;
			std::vector<VkPhysicalDevice>  phyDevices;
			uint32_t availGraphicQueueFamily = -1;
			rst = vkEnumeratePhysicalDevices( _inst, &nDevice, nullptr);
			if (rst != VK_SUCCESS || nDevice == 0)
			{
				return false;
			}
			phyDevices.resize(nDevice);
			rst = vkEnumeratePhysicalDevices(_inst, &nDevice, &phyDevices[0]);
			//
			printf("\n ###### List of physical devices : ######\n");
			for (auto& phyDevice : phyDevices)
			{
				VkPhysicalDeviceProperties props;
				vkGetPhysicalDeviceProperties(phyDevice, &props);
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
			vkGetPhysicalDeviceFeatures(physicalDevice, &features);
			vkGetPhysicalDeviceProperties(physicalDevice, &props);
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
			vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, nullptr);
			if (queueFamilyCount == 0)
			{
				return false;
			}
			// select an available queue family
			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
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
			device_._graphicQueueFamily = availGraphicQueueFamily;
			device_._host = physicalDevice;
			// create logical device
			std::vector<float> queuePriorities = { 1.0f };
			VkDeviceQueueCreateInfo queueCreateInfo = {
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
				nullptr,                                        // const void                  *pNext
				0,                                              // VkDeviceQueueCreateFlags     flags
				device_._graphicQueueFamily,                    // uint32_t                     queueFamilyIndex
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
			rst = vkCreateDevice(device_._host, &deviceCreateInfo, nullptr, &device_._id);
			//
			if (rst != VK_SUCCESS)
				return false;
			// Load device level api
#define VULKAN_DEVICE_API(api)\
	api = (PFN_##api)vkGetDeviceProcAddr(device_._id, #api);\
	if ( nullptr == api ){\
      std::cout << "Could not load device level function: " << #api << "!" << std::endl;\
      return false;\
    }
#include "inl/deviceFunctions.inl"
			return true;
		}
	}

	

}