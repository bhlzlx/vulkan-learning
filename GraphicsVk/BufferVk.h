#pragma once
#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"
#include "clGraphic.h"
#include <vector>

namespace clannad
{
	namespace vulkan
	{
		class Device;
		class Texture2D;
		class Buffer
		{
		public:
			enum UsageFlags
			{
				UsageTransferSrc = 0x00000001,
				UsageTransferDst = 0x00000002,
				UsageUniformTexel = 0x00000004,
				UsageStorageTexel = 0x00000008,
				UsageUniform = 0x00000010,
				UsageStorage = 0x00000020,
				UsageIndex = 0x00000040,
				UsageVertex = 0x00000080,
				UsageIndirect = 0x00000100,
			};
			enum SharingMode
			{
				ShairingModeExclusive = VK_SHARING_MODE_EXCLUSIVE,
				ShairingModeConcurrent = VK_SHARING_MODE_CONCURRENT,
			};
		private:
			VkBuffer _id;
			VkDeviceMemory _memory;
			size_t _size;
			//
			Device* _host;
			//
			uint32_t _usage;
			SharingMode _sharingMode;
			//
		public:
			static Buffer* Create( Device* _device, size_t _size, uint32_t _usage, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateStorageBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateVertexBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateIndexBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			void bufferData(void* _data, size_t _size, size_t _offset);
			void release();
		};

		class DataBlitManager
		{
		public:
			enum BlitType
			{
				BlitBuffer2Buffer,
				BlitBuffer2Image,
				BlitImage2Buffer,
				BlitImage2Image
			};

			struct BlitInfomation
			{
				BlitType type;
				union {
					struct
					{
						Buffer* src;
						Buffer* dst;
						size_t srcOffset;
						size_t dstOffset;
						size_t size;
					}b2b;
					struct 
					{
						Buffer* buffer;
						Texture2D * texture;
						size_t bufferOffset;
						ClRect<size_t> region;
					}b2t;
					struct 
					{
						Buffer* buffer;
						Texture2D * texture;
						size_t bufferOffset;
						ClRect<size_t> region;
					}t2b;
					struct
					{
						Texture2D * src;
						ClRect<size_t> srcRegion;
						Texture2D * dst;
						ClRect<size_t> dstRegion;
					}t2t;
				};
			};
		private:
			std::vector<BlitInfomation> _vecCommited;
			std::vector<BlitInfomation> _vecNeedCommit;
			void copyToBuffer( Buffer* _src, size_t _offsetSrc, Buffer* _dst, size_t _offsetDst, size_t _size );
			//void copyToImage( Buffer* _src, );
		};
	}
}
