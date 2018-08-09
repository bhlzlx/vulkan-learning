#pragma once
#include <VkApiLoader.h>

namespace clannad
{
	namespace vulkan
	{
		class View;
		class Device
		{
		friend class View;
		private:
			VkDevice _id;
			DeviceApi _api;
			VkPhysicalDevice _host;
			uint32_t _graphicQueueFamily;
			uint32_t _presentQueueFamily;
			//
			View* _currentView;
		public:
			Device(VkDevice _device, DeviceApi _api, VkPhysicalDevice _phyDevice, uint32_t _graphicQueue);
			~Device();
			//
			bool attachView(View* _view);
		};

	}
}


