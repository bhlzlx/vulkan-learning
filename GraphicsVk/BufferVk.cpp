#include "BufferVk.h"
#include "DeviceVk.h"
#include <memory.h>

namespace clannad
{
	namespace vulkan
	{
		Buffer* Buffer::Create(Device* _device, size_t _size, uint32_t _usage, SharingMode _sharingMode)
		{
			VkBuffer id;
			VkBufferCreateInfo buffInfo;
			buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffInfo.size = _size;
			buffInfo.usage = _usage;
			buffInfo.sharingMode = (VkSharingMode)_sharingMode;

			VkResult rst = _device->vkCreateBuffer(_device->_id, &buffInfo, nullptr, &id);
			if (rst != VK_SUCCESS)
				return nullptr;
			// 分配内存
			VkMemoryRequirements require;
			_device->vkGetBufferMemoryRequirements(_device->_id, id, &require);

			VkMemoryAllocateInfo allocInfo;
			allocInfo.allocationSize = require.size;
			allocInfo.memoryTypeIndex = _device->getMemoryType(require.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			VkDeviceMemory mem;
			_device->vkAllocateMemory(_device->_id, &allocInfo, nullptr, &mem);
			// 绑定内存
			_device->vkBindBufferMemory(_device->_id, id, mem, 0);
			//
			Buffer * buffer = new Buffer();
			buffer->_host = _device;
			buffer->_id = id;
			buffer->_usage = _usage;
			buffer->_sharingMode = _sharingMode;
			buffer->_memory = mem;
			buffer->_size = _size;
			//
			return buffer;
		}

		void Buffer::bufferData(void* _data, size_t _size, size_t _offset)
		{
			void * rawPtr = nullptr;
			_host->vkMapMemory(_host->_id, _memory, _offset, _size, (VkMemoryMapFlags)0, &rawPtr);
			memcpy(rawPtr, _data, _size);
			_host->vkUnmapMemory(_host->_id, _memory);
		}

		void Buffer::release()
		{
			_host->vkDestroyBuffer(_host->_id, _id, nullptr);
			_host->vkFreeMemory(_host->_id, _memory, nullptr);
			delete this;
		}

		Buffer* Buffer::CreateStorageBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageStorage, _sharingMode);
			return buffer;
		}

	}

	
}

