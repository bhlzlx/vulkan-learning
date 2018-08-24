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

		inline VkCullModeFlags mappintCullMode(ClCullMode _cullMode)
		{
			switch (_cullMode)
			{
			case clannad::ClCullNone:
				return VkCullModeFlagBits::VK_CULL_MODE_NONE;
			case clannad::ClCullBack:
				return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
			case clannad::ClCullFront:
				return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
			default:
				break;
			}
			return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		}

		inline VkPolygonMode  mappingPolygonMode(ClPolygonMode _mode)
		{
			switch (_mode)
			{
			case clannad::ClPolygonModePoint:
				return VK_POLYGON_MODE_POINT;
			case clannad::ClPolygonModeWireframe:
				return VK_POLYGON_MODE_LINE;
			case clannad::ClPolygonModeFill:
				return VK_POLYGON_MODE_FILL;
			default:
				break;
			}
			return VK_POLYGON_MODE_FILL;
		}

		inline VkCompareOp mappingCompareFunction(ClCompareFunction _func)
		{
			switch (_func)
			{
			case clannad::ClCompareAlways:
				return VK_COMPARE_OP_ALWAYS;
			case clannad::ClCompareNever:
				return VK_COMPARE_OP_NEVER;
			case clannad::ClCompareLess:
				return VK_COMPARE_OP_LESS;
			case clannad::ClCompareLessEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case clannad::ClCompareEqual:
				return VK_COMPARE_OP_EQUAL;
			case clannad::ClCompareGreaterEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case clannad::ClCompareGreater:
				return VK_COMPARE_OP_GREATER;
			default: 
				return VK_COMPARE_OP_ALWAYS;
			}
		}

		inline VkStencilOp mappingStencilOp(ClStencilOperator _oper)
		{
			switch (_oper)
			{
			case clannad::ClStencilOpKeep:
				return VK_STENCIL_OP_KEEP;
			case clannad::ClStencilOpZero:
				return VK_STENCIL_OP_ZERO;
			case clannad::ClStencilOpReplace:
				return VK_STENCIL_OP_REPLACE;
			case clannad::ClStencilOpInc:
				return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case clannad::ClStencilOpDec:
				return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			case clannad::ClStencilInvert:
				return VK_STENCIL_OP_INVERT;
			default:
			}
			return VK_STENCIL_OP_KEEP;
		}

		inline VkBlendFactor mappingBlendFactor(ClBlendFactor _factor)
		{
			switch (_factor)
			{
			case clannad::ClBlendFactorZero: return VK_BLEND_FACTOR_ZERO;
			case clannad::ClBlendFactorOne: return VK_BLEND_FACTOR_ONE;
			case clannad::ClBlendFactorSourceColor: return VK_BLEND_FACTOR_SRC_COLOR;
			case clannad::ClBlendFactorSourceAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
			case clannad::ClBlendFactorDestinationColor: return VK_BLEND_FACTOR_DST_COLOR;
			case clannad::ClBlendFactorDestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case clannad::ClBlendFactorOneMinusSourceColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case clannad::ClBlendFactorOneMinusSourceAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case clannad::ClBlendFactorOneMinusDestinationColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case clannad::ClBlendFactorOneMinusDestinationAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case clannad::ClBlendFactorSourceAlphaMinor: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case clannad::ClBlendFactorColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case clannad::ClBlendFactorOneMinusColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case clannad::ClBlendFactorAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case clannad::ClBlendFactorOneMinusAlpha: VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case clannad::ClBlendFactorSource1Color: VK_BLEND_FACTOR_SRC1_COLOR;
			case clannad::ClBlendFactorOneMinusSource1Color: VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case clannad::ClBlendFactorSource1Alpha: VK_BLEND_FACTOR_SRC1_ALPHA;
			case clannad::ClBlendFactorOneMinusSource1Alpha: VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			default:
				break;
			}
			return VK_BLEND_FACTOR_MAX_ENUM;
		}

		inline VkBlendOp mappingBlendOp(ClBlendOperation _op)
		{
			switch (_op)
			{
			case clannad::ClBlendOperationAdd: return VK_BLEND_OP_ADD;
			case clannad::ClBlendOperationSubtract: return VK_BLEND_OP_SUBTRACT;
			case clannad::ClBlendOperationReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case clannad::ClBlendOperationMin: return VK_BLEND_OP_MIN;
			case clannad::ClBlendOperationMax: return VK_BLEND_OP_MAX;
			default:
				break;
			}
			return VK_BLEND_OP_MAX_ENUM;
		}
	}
}
