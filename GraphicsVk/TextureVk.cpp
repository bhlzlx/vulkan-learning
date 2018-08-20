#include "TextureVk.h"
#include "DeviceVk.h"
#include "ViewVk.h"
#include "BufferVk.h"
#include "vkTypeMapping.h"


namespace clannad
{
	namespace vulkan
	{
		const VkInstance& GetVkInstance();
		//

		void Texture2D::subImage(void * _data, const ClRect<uint32_t>& _rect)
		{
			Buffer * buffer = Buffer::CreateStorageBuffer(_host, mappingPixelSize(_desc.format) * (_rect.right - _rect.left) * (_rect.top - _rect.bottom));
			DataBlitManager::Instance()->blockedCopyToImage(_host, buffer, 0, buffer->size(), this, _rect);
		}

		void Texture2D::release()
		{
			if (_imageView)
			{
				vkDestroyImageView(*_host, _imageView, nullptr);
			}
			if (_id)
			{
				vkDestroyImage(*_host, _id, nullptr);
			}
			if (_memory)
			{
				vkFreeMemory(*_host, _memory, nullptr);
			}
			delete this;
		}

		Texture2D * Texture2D::Create(Device * _device, const ClTextureDesc & _desc)
		{
			VkImageCreateInfo info =
			{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				0, // not a cube map 	VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
				VK_IMAGE_TYPE_2D,
				mappingPixelFormat(_desc.format), // 格式
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
			// Create ImageView
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = tex->_id;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			createInfo.pNext = nullptr;
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = mappingPixelFormat(_desc.format);
			createInfo.flags = mappingImageUsage(_desc.usage);

			VkImageView imageView;
			rst = vkCreateImageView(*_device, &createInfo, nullptr, &imageView);
			tex->_imageView = imageView;
			//
			return tex;
		}

		//
		Sampler2D* Sampler2D::Create(Device* _host, const ClSamplerDesc& _desc)
		{
			VkSamplerCreateInfo samplerInfo = {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = mappingAddressMode(_desc.U);
			samplerInfo.addressModeV = mappingAddressMode(_desc.V);
			samplerInfo.addressModeW = mappingAddressMode(_desc.W);
			samplerInfo.magFilter = mappingFilterMode(_desc.magFilter);
			samplerInfo.minFilter = mappingFilterMode(_desc.minFilter);
			samplerInfo.mipmapMode = (VkSamplerMipmapMode)mappingFilterMode(_desc.mipFilter);
			samplerInfo.borderColor = VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.mipLodBias = 0.0f;

			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.pNext = nullptr;
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = 16;
			//
			VkSampler sampler;
			VkResult rst = vkCreateSampler(*_host, &samplerInfo, nullptr, &sampler);
			if (rst == VK_SUCCESS)
			{
				Sampler2D* s = new Sampler2D();
				s->_desc = _desc;
				s->_host = _host;
				s->_id = sampler;
				return s;
			}
			return nullptr;
		}
	}
}