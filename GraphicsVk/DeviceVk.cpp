#include "DeviceVk.h"
#include <vector>
#include "ViewVk.h"

namespace clannad
{
	namespace vulkan
	{
		Device::Device(const DeviceLoadData& _loadData)
		{
			this->_graphicQueueFamily = _loadData._graphicQueueFamily;
			this->_host = _loadData._host;
			this->_id = _loadData._id;
		}

		Device::~Device()
		{
		}

		bool Device::attachView(View * _view)
		{
			_view->setHost(this);
			_currentView = _view;
			//
			VkSurfaceKHR surface = _view->getSurface();

			//const Instance& inst = GetVkInstance();
			//
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR( _host, _graphicQueueFamily, surface, &presentSupport);
			if (presentSupport)
			{
				_presentQueueFamily = _graphicQueueFamily;
				return true;
			}
			//
			uint32_t queueFamilyCount;
			vkGetPhysicalDeviceQueueFamilyProperties( _host, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(_host, &queueFamilyCount, queueFamilyProperties.data());

			uint32_t index = 0;
			for (const VkQueueFamilyProperties& property : queueFamilyProperties)
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(_host, index, surface, &presentSupport);
				if (presentSupport)
				{
					_presentQueueFamily = index;
					return true;
				}
				++index;
			}
			//
			_view->setHost(nullptr);
			return false;
		}

		uint32_t Device::getMemoryType(uint32_t _memTypeBits, VkMemoryPropertyFlags properties)
		{
			const auto inst = GetVkInstance();
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(_host, &memProperties);
			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if (_memTypeBits & (1 << i))
				{
					if ((memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
					{
						return i;
					}
				}
			}
			throw std::runtime_error("failed to find suitable memory type!");
			return 0;
		}

	}
}

