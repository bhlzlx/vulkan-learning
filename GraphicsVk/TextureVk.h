#pragma once

#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"
#include "clGraphic.h"
#include <vector>
#include <utility>

namespace clannad
{
	namespace vulkan
	{
		class Device;
		//
		/*struct TransitionBuffer
		{
			VkBuffer buffer;
			VkDeviceSize size;
			void Initialize( Device* _device );
		};*/
		class Texture2D
		{
		private:
			VkImage _id = nullptr;
			VkDeviceMemory _memory;
			Device* _host = nullptr;
			ClTextureDesc _desc;
			//
			Texture2D():_id(nullptr)
			{
			}
			~Texture2D()
			{
			}
		public:
			const ClTextureDesc& getDesc()const
			{
				return _desc;
			}
			void subImage(void * _data, const ClRect<uint32_t>& _rect);
			operator VkImage() {
				return _id;
			}
			static Texture2D* Create(Device* _device, const ClTextureDesc& _desc);
		};
	}
}

