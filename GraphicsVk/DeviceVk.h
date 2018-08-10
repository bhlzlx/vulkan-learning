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
			union
			{
				DeviceApi _api;
				struct {
#define VULKAN_DEVICE_API( FUNCTION ) PFN_##FUNCTION FUNCTION;
#include "inl/deviceFunctions.inl"
				};
			};
			
			VkPhysicalDevice _host;
			uint32_t _graphicQueueFamily;
			uint32_t _presentQueueFamily;
			//
			View* _currentView;
		public:
			Device( const DeviceLoadData& _loadData );
			~Device();
			//
			bool attachView(View* _view);
			// 
			operator VkDevice()
			{
				return _id;
			}
		};

	}
}


