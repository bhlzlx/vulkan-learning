#pragma once
#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"
#include "clGraphic.h"
#include <vector>
#include <mutex>
#include <functional>

namespace clannad
{
	namespace vulkan
	{
		class Device;
		class Texture2D;
		class Buffer
		{
			friend class DataBlitManager;
			friend class DataBlitManager;
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
			enum MemoryType
			{
				MemoryTypeDeviceAccess = 0x1,
				MemoryTypeHostAccess = 0x2
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
			MemoryType _memType;
			//
		public:
			//************************************
			// Method:    Create
			// FullName:  clannad::vulkan::Buffer::Create
			// Access:    public static 
			// Returns:   clannad::vulkan::Buffer*
			// Qualifier:
			// Parameter: Device * _device the host device
			// Parameter: size_t _size
			// Parameter: uint32_t _usage vertexbuffer, indexbuffer, uniformbufer, storage
			// Parameter: MemoryType _memType device access(GPU only) or host access(CPU support)
			// Parameter: SharingMode _sharingMode can be used in multi command queue concurrently??
			//************************************
			static Buffer* Create( Device* _device, size_t _size, uint32_t _usage, MemoryType _memType = MemoryTypeDeviceAccess, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateStageBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateStorageBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateVertexBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateIndexBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			static Buffer* CreateUniformBuffer(Device* _device, size_t _size, SharingMode _sharingMode = ShairingModeExclusive);
			void bufferData(void* _data, size_t _size, size_t _offset);
			bool map(void ** _ptr);
			void unmap(size_t _offset, size_t _size);
			void release();
			size_t size() {
				return _size;
			}
			operator VkBuffer(){
				return _id;
			}
		};

		class DataBlitManager
		{
		public:
			enum BlitType
			{
				BlitBuffer2Buffer = 0,
				BlitBuffer2Image = 1,
				BlitImage2Buffer = 2,
				BlitImage2Image = 3
			};

			struct BlitOperation
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
					}buf2buf;
					struct 
					{
						Buffer* buffer;
						Texture2D * texture;
						size_t bufferOffset;
						ClRect<uint32_t> region;
					}buf2tex;
					struct 
					{
						Texture2D * texture;
						Buffer* buffer;
						size_t bufferOffset;
						size_t size;
						ClRect<uint32_t> region;
					}tex2buf;
					struct
					{
						Texture2D * src;
						Texture2D * dst;
						ClRect<uint32_t> srcRegion;
						ClRect<uint32_t> dstRegion;
					}tex2tex;
					struct
					{
						void* src;
						void* dst;
					};
					//
				};
				std::function<void( void* _src, void * _dst)> completeHandler;
				BlitOperation(){}
			public:
				void Encode( VkCommandBuffer _cmd );
			};
		private:
			std::mutex _mutex;
			std::vector<BlitOperation> _vecCommited;
			std::vector<BlitOperation> _vecNeedCommit;
		private:
		public:
			void copyToBuffer( Buffer* _src, size_t _offsetSrc, Buffer* _dst, size_t _offsetDst, size_t _size, std::function<void(void* _src, void * _dst)> _completeHandler = nullptr);
			void copyToBuffer(Texture2D* _src, ClRect<uint32_t> _region, Buffer* _dst, size_t _offset, std::function<void(void* _src, void * _dst)> _completeHandler = nullptr);
			void copyToTexture2D(Buffer* _src, size_t _offset, size_t _size, Texture2D* _dst, ClRect<uint32_t> _region, std::function<void(void* _src, void * _dst)> _completeHandler = nullptr);
			void copyToTexture2D(Texture2D* _src, ClRect<uint32_t> _srcRegion, Texture2D* _dst, ClRect<uint32_t> _dstRegion, std::function<void(void* _src, void * _dst)> _completeHandler = nullptr);
			//
			void queueBlitCommand(Device* _device);
			// blocked function
			void blockedCopyToImage(Device* _device, Buffer* _src, size_t _offset, size_t _size, Texture2D* _dst, ClRect<uint32_t> _region);
		public:
			static DataBlitManager* _Instance;

			static DataBlitManager* Instance()
			{
				if( !_Instance)
					_Instance = new DataBlitManager();
				return _Instance;
			}
		};

	}
}
