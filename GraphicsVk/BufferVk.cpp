#include "BufferVk.h"
#include "DeviceVk.h"
#include "ViewVk.h"
#include "TextureVk.h"
#include "vkTypeMapping.h"
#include <memory.h>

namespace clannad
{
	namespace vulkan
	{
		Buffer* Buffer::Create(Device* _device, size_t _size, uint32_t _usage, MemoryType _memType, SharingMode _sharingMode)
		{
			VkBuffer id;
			VkBufferCreateInfo buffInfo;
			buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffInfo.size = _size;
			buffInfo.usage = _usage;
			buffInfo.sharingMode = (VkSharingMode)_sharingMode;

			VkResult rst = vkCreateBuffer(_device->_id, &buffInfo, nullptr, &id);
			if (rst != VK_SUCCESS)
				return nullptr;
			// allocate memory
			VkMemoryRequirements require;
			vkGetBufferMemoryRequirements(_device->_id, id, &require);

			VkMemoryAllocateInfo allocInfo;
			allocInfo.allocationSize = require.size;

			uint32_t memFlagBits = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			if (_memType == MemoryTypeDeviceAccess)
			{
				memFlagBits = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			}
			else if( _memType == MemoryTypeHostAccess )
			{
				memFlagBits = VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			}			
			allocInfo.memoryTypeIndex = _device->getMemoryType(require.memoryTypeBits, memFlagBits);
			
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			VkDeviceMemory mem;
			vkAllocateMemory(_device->_id, &allocInfo, nullptr, &mem);
			// bind memory to buffer object
			vkBindBufferMemory(_device->_id, id, mem, 0);
			//
			Buffer * buffer = new Buffer();
			buffer->_host = _device;
			buffer->_id = id;
			buffer->_usage = _usage;
			buffer->_sharingMode = _sharingMode;
			buffer->_memory = mem;
			buffer->_size = _size;
			buffer->_memType = _memType;
			//
			return buffer;
		}


		void Buffer::bufferData(void* _data, size_t _size, size_t _offset)
		{
			void * rawPtr = nullptr;
			vkMapMemory(_host->_id, _memory, _offset, _size, (VkMemoryMapFlags)0, &rawPtr);
			if (rawPtr)
			{
				memcpy(rawPtr, _data, _size);
				vkUnmapMemory(_host->_id, _memory);
			}
		}

		bool Buffer::map(void ** _ptr)
		{
			void * rawPtr = nullptr;
			vkMapMemory(*_host, _memory, 0, _size, (VkMemoryMapFlags)0, &rawPtr);
			if (rawPtr)
			{
				return true;
			}
		}

		void Buffer::unmap( size_t _offset, size_t _size)
		{
			VkMappedMemoryRange mapRange;
			mapRange.memory = _memory;
			mapRange.offset = _offset;
			mapRange.pNext = nullptr;
			mapRange.size = _size;
			mapRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			vkFlushMappedMemoryRanges(*_host, 1, &mapRange);
			// vkInvalidateMappedMemoryRanges
			vkUnmapMemory(_host->_id, _memory);
		}

		void Buffer::release()
		{
			vkDestroyBuffer(_host->_id, _id, nullptr);
			vkFreeMemory(_host->_id, _memory, nullptr);
			delete this;
		}

		clannad::vulkan::Buffer* Buffer::CreateStageBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageStorage, MemoryTypeHostAccess);
			return buffer;
		}

		Buffer* Buffer::CreateStorageBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageStorage, MemoryTypeDeviceAccess, _sharingMode);
			return buffer;
		}

		clannad::vulkan::Buffer* Buffer::CreateVertexBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageVertex, MemoryTypeDeviceAccess, _sharingMode);
			return buffer;
		}

		clannad::vulkan::Buffer* Buffer::CreateIndexBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageIndex, MemoryTypeDeviceAccess, _sharingMode);
			return buffer;
		}

		clannad::vulkan::Buffer* Buffer::CreateUniformBuffer(Device* _device, size_t _size, SharingMode _sharingMode /*= ShairingModeExclusive*/)
		{
			Buffer* buffer = Create(_device, _size, UsageUniform, MemoryTypeDeviceAccess, _sharingMode);
			return buffer;
		}

		//
		DataBlitManager* DataBlitManager::_Instance = nullptr;

		//
		void DataBlitManager::copyToBuffer(Buffer* _src, size_t _offsetSrc, Buffer* _dst, size_t _offsetDst, size_t _size, std::function<void(void* _src, void * _dst)> _completeHandler)
		{
			_mutex.lock();

			BlitOperation oper;
			oper.type = BlitBuffer2Buffer;
			oper.buf2buf.size = _size;
			oper.buf2buf.src = _src;
			oper.buf2buf.srcOffset = _offsetSrc;
			oper.buf2buf.dst = _dst;
			oper.buf2buf.dstOffset = _offsetDst;
			oper.completeHandler = _completeHandler;
			this->_vecNeedCommit.push_back(oper);

			_mutex.unlock();
		}

		void DataBlitManager::copyToBuffer(Texture2D* _src, ClRect<uint32_t> _region, Buffer* _dst, size_t _offset, std::function<void(void* _src, void * _dst)> _completeHandler)
		{
			_mutex.lock();

			BlitOperation oper;
			oper.type = BlitImage2Buffer;
			oper.tex2buf.texture = _src;
			oper.tex2buf.region = _region;
			oper.tex2buf.buffer = _dst;
			oper.tex2buf.bufferOffset = _offset;
			oper.completeHandler = _completeHandler;
			this->_vecNeedCommit.push_back(oper);

			_mutex.unlock();
		}

		void DataBlitManager::copyToTexture2D(Buffer* _src, size_t _offset, size_t _size, Texture2D* _dst, ClRect<uint32_t> _region, std::function<void(void* _src, void * _dst)> _completeHandler)
		{
			_mutex.lock();

			BlitOperation oper;
			oper.type = BlitBuffer2Image;
			oper.buf2tex.buffer = _src;
			oper.buf2tex.bufferOffset = _offset;
			oper.buf2tex.region = _region;
			oper.buf2tex.texture = _dst;
			oper.completeHandler = _completeHandler;

			_mutex.unlock();
		}

		void DataBlitManager::copyToTexture2D(Texture2D* _src, ClRect<uint32_t> _srcRegion, Texture2D* _dst, ClRect<uint32_t> _dstRegion, std::function<void(void* _src, void * _dst)>)
		{
			_mutex.lock();

			BlitOperation oper;
			oper.type = BlitImage2Image;
			oper.tex2tex.src = _src;
			oper.tex2tex.srcRegion = _srcRegion;
			oper.tex2tex.dst = _dst;
			oper.tex2tex.dstRegion = _dstRegion;
			oper.completeHandler = nullptr;

			_mutex.unlock();
		}

		void DataBlitManager::queueBlitCommand(Device* _device)
		{
			if (_vecCommited.size())
			{
				for (auto& oper : _vecCommited)
				{
					oper.completeHandler(oper.src, oper.dst);
				}
			}
			_mutex.lock();
			//
			if (_vecNeedCommit.size())
			{
				VkQueue graphicQueue = _device->getView()->getGraphicQueue();
				VkCommandPool commandPool = _device->getView()->getCommandPool();
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandPool;
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer commandBuffer;
				vkAllocateCommandBuffers( *_device , &allocInfo, &commandBuffer);
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				vkBeginCommandBuffer(commandBuffer, &beginInfo);
				// 
				for (auto& oper : _vecNeedCommit)
				{
					oper.Encode(commandBuffer);
				}
				//
				vkEndCommandBuffer(commandBuffer);
				/*VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;
				vkQueueSubmit(graphicQueue, 1, &submitInfo, VK_NULL_HANDLE);
				*/
				std::swap(_vecNeedCommit, _vecCommited);
				_vecNeedCommit.clear();
			}
			_mutex.unlock();
			//
		}

		void DataBlitManager::blockedCopyToImage( Device* _device, Buffer* _src, size_t _offset, size_t _size, Texture2D* _dst, ClRect<uint32_t> _region)
		{
			VkQueue graphicQueue = _device->getView()->getGraphicQueue();
			VkCommandPool commandPool = _device->getView()->getCommandPool();
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = commandPool;
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(*_device, &allocInfo, &commandBuffer);
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(commandBuffer, &beginInfo);


			BlitOperation oper;
			oper.type = BlitBuffer2Image;
			oper.buf2tex.buffer = _src;
			oper.buf2tex.bufferOffset = _offset;
			oper.buf2tex.region = _region;
			oper.buf2tex.texture = _dst;
			oper.Encode(commandBuffer);

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(graphicQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(graphicQueue);

			vkFreeCommandBuffers(*_device, commandPool, 1, &commandBuffer);
		}

		void DataBlitManager::BlitOperation::Encode(VkCommandBuffer _cmd)
		{
			switch (type)
			{
			case BlitBuffer2Buffer:
			{
				auto data = buf2buf;
				VkBufferCopy cp;
				cp.dstOffset = data.dstOffset;
				cp.srcOffset = data.srcOffset;
				cp.size = data.size;
				vkCmdCopyBuffer(_cmd, *data.src, *data.dst, 1, &cp);
				break;
			}
			case BlitBuffer2Image:
			{
				auto data = buf2tex;
				VkBufferImageCopy cp;
				cp.imageOffset.x = data.region.left;
				cp.imageOffset.y = data.region.bottom;
				cp.imageOffset.z = 0;
				cp.bufferOffset = data.bufferOffset;
				cp.bufferRowLength = 0;
				cp.bufferImageHeight = 0;
				cp.imageExtent.depth = 0;
				cp.imageExtent.width = data.region.right - data.region.left;
				cp.imageExtent.height = data.region.top - data.region.bottom;
				cp.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				cp.imageSubresource.baseArrayLayer = 0;
				cp.imageSubresource.layerCount = 1;
				cp.imageSubresource.mipLevel = 0;
				cp.imageSubresource.layerCount = 1;
				//
				vkCmdCopyBufferToImage(_cmd, *data.buffer, *data.texture, VK_IMAGE_LAYOUT_GENERAL, 1, &cp);
				//
				break;
			}
			case BlitImage2Buffer:
			{
				auto data = tex2buf;
				VkBufferImageCopy cp;
				cp.imageOffset.x = data.region.left;
				cp.imageOffset.y = data.region.bottom;
				cp.imageOffset.z = 0;
				cp.bufferOffset = data.bufferOffset;
				cp.bufferRowLength = 0;
				cp.bufferImageHeight = 0;
				cp.imageExtent.depth = 0;
				cp.imageExtent.width = data.region.right - data.region.left;
				cp.imageExtent.height = data.region.top - data.region.bottom;
				cp.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				cp.imageSubresource.baseArrayLayer = 0;
				cp.imageSubresource.layerCount = 1;
				cp.imageSubresource.mipLevel = 0;
				cp.imageSubresource.layerCount = 1;
				//
				vkCmdCopyImageToBuffer(_cmd, *data.texture, VK_IMAGE_LAYOUT_GENERAL, *data.buffer, 1, &cp);
				break;
			}
			case BlitImage2Image:
			{
				auto data = tex2tex;

				VkImageCopy cp;
				cp.dstOffset.x = data.dstRegion.left;
				cp.dstOffset.y = data.dstRegion.bottom;
				cp.dstOffset.z = 0;
				cp.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				cp.dstSubresource.baseArrayLayer = 0;
				cp.dstSubresource.layerCount = 1;
				cp.dstSubresource.mipLevel = 0;
				cp.extent.depth = 0;
				cp.extent.width = data.dstRegion.right - data.dstRegion.left;
				cp.extent.height = data.dstRegion.top - data.dstRegion.bottom;
				cp.extent.depth = 0;

				cp.srcOffset.x = data.srcRegion.left;
				cp.srcOffset.y = data.srcRegion.bottom;
				cp.srcOffset.z = 0;
				cp.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				cp.srcSubresource.baseArrayLayer = 0;
				cp.srcSubresource.layerCount = 1;
				cp.srcSubresource.mipLevel = 0;
				//
				vkCmdCopyImage(_cmd, *data.src, VK_IMAGE_LAYOUT_GENERAL, *data.dst, VK_IMAGE_LAYOUT_GENERAL, 1, &cp);
				break;
			}
			}
		}

		

	}
}

