#include "DeviceVk.h"
#include <vector>
#include "ViewVk.h"

namespace clannad
{
	namespace vulkan
	{
		Device::Device(const DeviceLoadData& _loadData)
		{
			this->_api = _loadData._api;
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

			const Instance& inst = GetVkInstance();
			//
			VkBool32 presentSupport = false;
			inst.vkGetPhysicalDeviceSurfaceSupportKHR( _host, _graphicQueueFamily, surface, &presentSupport);
			if (presentSupport)
			{
				_presentQueueFamily = _graphicQueueFamily;
				return true;
			}
			//
			uint32_t queueFamilyCount;
			inst.vkGetPhysicalDeviceQueueFamilyProperties( _host, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			inst.vkGetPhysicalDeviceQueueFamilyProperties(_host, &queueFamilyCount, queueFamilyProperties.data());

			uint32_t index = 0;
			for (const VkQueueFamilyProperties& property : queueFamilyProperties)
			{
				VkBool32 presentSupport = false;
				inst.vkGetPhysicalDeviceSurfaceSupportKHR(_host, index, surface, &presentSupport);
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

