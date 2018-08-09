#include "DeviceVk.h"
#include <vector>
#include "ViewVk.h"

namespace clannad
{
	namespace vulkan
	{
		Device::Device(VkDevice _device, DeviceApi _api, VkPhysicalDevice _phyDevice, uint32_t _graphicQueue)
		{
			this->_api = _api;
			this->_graphicQueueFamily = _graphicQueue;
			this->_host = _phyDevice;
			this->_id = _device;
			///_api.vkCreateCommandPool()
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
			const InstanceApi *instApi;
			VkInstance inst = GetVkInstance(instApi);
			//
			VkBool32 presentSupport = false;
			instApi->vkGetPhysicalDeviceSurfaceSupportKHR( _host, _graphicQueueFamily, surface, &presentSupport);
			if (presentSupport)
			{
				_presentQueueFamily = _graphicQueueFamily;
				return true;
			}
			//
			uint32_t queueFamilyCount;
			instApi->vkGetPhysicalDeviceQueueFamilyProperties( _host, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			instApi->vkGetPhysicalDeviceQueueFamilyProperties(_host, &queueFamilyCount, queueFamilyProperties.data());

			uint32_t index = 0;
			for (const VkQueueFamilyProperties& property : queueFamilyProperties)
			{
				VkBool32 presentSupport = false;
				instApi->vkGetPhysicalDeviceSurfaceSupportKHR(_host, index, surface, &presentSupport);
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

	}
}

