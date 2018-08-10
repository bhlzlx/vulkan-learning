#pragma once
#include <vulkan/vulkan.h>

namespace clannad
{
	namespace vulkan
	{
		class Device;
		class Buffer
		{
		private:
			VkBuffer _id;
			//
			Device* _host;
			//
		public:
			void setData(void* _data, size_t _size, size_t _offset);
			void release();
		};
	}
}
