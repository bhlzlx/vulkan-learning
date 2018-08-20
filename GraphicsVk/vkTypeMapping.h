#pragma once

#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.h"
#include "ClGraphic.h"

namespace clannad
{
	namespace vulkan
	{
		inline VkFormat mappingVertexFormat(ClVertexFormat _format)
		{
			VkFormat formats[] =
			{
				VK_FORMAT_UNDEFINED,

				VK_FORMAT_R8_UINT,
				VK_FORMAT_R8G8_UINT,
				VK_FORMAT_R8G8B8_UINT,
				VK_FORMAT_R8G8B8A8_UINT,
				VK_FORMAT_R8_SINT,
				VK_FORMAT_R8G8B8A8_SINT,
				VK_FORMAT_R8G8B8A8_SINT,
				VK_FORMAT_R8G8B8A8_SINT,

				VK_FORMAT_UNDEFINED,
				VK_FORMAT_R8G8_UNORM,
				VK_FORMAT_R8G8B8_UNORM,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_R8G8_SNORM,
				VK_FORMAT_R8G8B8_SNORM,
				VK_FORMAT_R8G8B8A8_SNORM,

				//
				VK_FORMAT_R16_UINT,
				VK_FORMAT_R16G16_UINT,
				VK_FORMAT_R16G16B16_UINT,
				VK_FORMAT_R16G16B16A16_UINT,
				VK_FORMAT_R16_SINT,
				VK_FORMAT_R16G16B16A16_SINT,
				VK_FORMAT_R16G16B16A16_SINT,
				VK_FORMAT_R16G16B16A16_SINT,

				VK_FORMAT_UNDEFINED,
				VK_FORMAT_R16G16_UNORM,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R16G16B16A16_UNORM,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_R16G16_SNORM,
				VK_FORMAT_R16G16B16_SNORM,
				VK_FORMAT_R16G16B16A16_SNORM,
				//
				VK_FORMAT_R32_UINT,
				VK_FORMAT_R32G32_UINT,
				VK_FORMAT_R32G32B32_UINT,
				VK_FORMAT_R32G32B32A32_UINT,
				VK_FORMAT_R32_SINT,
				VK_FORMAT_R32G32B32A32_SINT,
				VK_FORMAT_R32G32B32A32_SINT,
				VK_FORMAT_R32G32B32A32_SINT,

				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				//
				VK_FORMAT_R32_SFLOAT,
				VK_FORMAT_R32G32_SFLOAT,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				//
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
				VK_FORMAT_UNDEFINED,
			};

			static_assert(ClVertexFormatEnd == sizeof(formats) / sizeof(VkFormat), "");

			return formats[_format];
		}

		inline VkFormat mappingPixelFormat(ClPixelFormat _format)
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

		inline uint32_t mappingPixelSize(ClPixelFormat _format)
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

		inline VkImageUsageFlags mappingImageUsage(uint8 _usage)
		{
			return (VkImageUsageFlags)_usage;
		}

		inline VkSamplerAddressMode mappingAddressMode(ClTextureAddress _mode)
		{
			switch (_mode)
			{
			case clannad::ClTextureAddressWrap:
				return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case clannad::ClTextureAddressMirror:
				return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			case clannad::ClTextureAddressClamp:
				return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case clannad::ClTextureAddressClampOne:
			case clannad::ClTextureAddressClampZero:
				return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
			default:
				break;
			}
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}

		inline VkFilter mappingFilterMode(ClSamplerFilter _mode)
		{
			switch (_mode)
			{
			case clannad::ClFilterLinear:
				return VkFilter::VK_FILTER_LINEAR;
			case clannad::ClFilterPoint:
				return VkFilter::VK_FILTER_NEAREST;
			case clannad::ClFilerNone:
				return VkFilter::VK_FILTER_NEAREST;
			default:
				break;
			}
			return VkFilter::VK_FILTER_NEAREST;
		}
	}
}