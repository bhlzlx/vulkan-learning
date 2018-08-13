#include "TextureVk.h"
#include "DeviceVk.h"
#include "ViewVk.h"
#include "BufferVk.h"

namespace clannad
{
	namespace vulkan
	{
		const VkInstance& GetVkInstance();
		//
		VkFormat mappingFormat(ClPixelFormat _format)
		{
			VkFormat rst = VK_FORMAT_UNDEFINED;
			switch (_format)
			{
			case ClPixelFormat_RGBA8888_UNORM: rst = VK_FORMAT_R8G8B8A8_UNORM; break;
			case ClPixelFormat_RGBA8888_SNORM: rst = VK_FORMAT_R8G8B8A8_SNORM; break;
			case ClPixelFormat_RGBA4444_UNORM_PACKED: rst = VK_FORMAT_R4G4B4A4_UNORM_PACK16; break;
			case ClPixelFormat_RGB565_UNORM_PACKED: rst = VK_FORMAT_R5G6B5_UNORM_PACK16; break;
			case ClPixelFormat_Depth32F: rst = VK_FORMAT_D32_SFLOAT; break;
			case ClPixelFormat_Depth32Stencil8: rst = VK_FORMAT_D32_SFLOAT_S8_UINT; break;
			case ClPixelFormat_Stencil8: rst = VK_FORMAT_S8_UINT; break;
			case ClPixelFormat_Depth24Stencil8_PACKED: rst = VK_FORMAT_D24_UNORM_S8_UINT; break;
				//
			case ClPixelFormat_Depth_24F:
			case ClPixelFormat_BGRA4444_SNORM_PACKED:
			case ClPixelFormat_Invalid:
			default:
				break;
			}
			return rst;
		}

		uint32_t pixelSize(ClPixelFormat _format)
		{
			uint32_t sizeTable[] = {
				0,
				4,
				4,
				2,
				2,
				2,
				3,
				4,
				1,
				4,
				5
			};
			return sizeTable[_format];
		}

		VkImageUsageFlags mappingImageUsage(uint8 _usage)
		{
			return (VkImageUsageFlags)_usage;
		}

		void Texture2D::subImage(void * _data, const ClRect<uint32_t>& _rect)
		{
			Buffer * buffer = Buffer::CreateStorageBuffer(_host, pixelSize(_desc.format) * (_rect.right - _rect.left) * (_rect.top - _rect.bottom));
			DataBlitManager::Instance()->blockedCopyToImage(_host, buffer, 0, buffer->size(), this, _rect);
		}

		Texture2D * Texture2D::Create(Device * _device, const ClTextureDesc & _desc)
		{
			VkImageCreateInfo info =
			{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				0, // not a cube map 	VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
				VK_IMAGE_TYPE_2D,
				mappingFormat(_desc.format), // 格式
				{ _desc.width, _desc.height, 1 }, // 大小
				0, // mipmap
				1, // 有几层？
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				mappingImageUsage( _desc.usage ),
				VK_SHARING_MODE_EXCLUSIVE,
				0,
				nullptr,
				VK_IMAGE_LAYOUT_UNDEFINED
			};
			VkImage img;
			VkResult rst = vkCreateImage(_device->_id, &info, nullptr, &img);
			if (rst != VK_SUCCESS)
				return nullptr;
			//
			Texture2D* tex = new Texture2D();
			tex->_id = img;
			tex->_host = _device;
			tex->_desc = _desc;
			// 分配显存
			VkMemoryRequirements require;
			vkGetImageMemoryRequirements(_device->_id, tex->_id, &require);

			VkMemoryAllocateInfo allocInfo;
			allocInfo.allocationSize = require.size;
			allocInfo.memoryTypeIndex = _device->getMemoryType(require.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			//
			VkDeviceMemory mem;
			vkAllocateMemory(_device->_id, &allocInfo, nullptr, &mem);
			tex->_memory = mem;
			// 绑定显存
			vkBindImageMemory(_device->_id, tex->_id, mem, 0);
			//
			return tex;
		}
	}
}