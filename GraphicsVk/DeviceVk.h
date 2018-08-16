#pragma once
#include <VkApiLoader.h>

namespace clannad
{
	namespace vulkan
	{
		class View;

		class Device
		{
			friend class Buffer;
			friend class View;
			friend class Texture2D;
		private:
			VkDevice _id;

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
			View* getView()
			{
				return _currentView;
			}

			uint32_t getMemoryType(uint32_t _memTypeBits, VkMemoryPropertyFlags properties);
			// 
			operator VkDevice()
			{
				return _id;
			}
			VkDevice get()
			{
				return _id;
			}
		};

	}
}


