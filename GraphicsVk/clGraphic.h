#pragma once
#include <stdint.h>
#include <string>

namespace clannad
{
	typedef uint8_t byte;
	typedef uint16_t word;
	typedef uint32_t dword;
	typedef uint8_t uint8;
	typedef uint32_t uint32;
	typedef uint16_t uint16;
	typedef uint64_t uint64;

	typedef int8_t int8;
	typedef int32_t int32;
	typedef int16_t int16;
	typedef int64_t int64;

	enum ClPixelFormat:uint8
	{
		ClPixelFormat_Invalid = 0,
		ClPixelFormat_RGBA8888_UNORM,
		ClPixelFormat_RGBA8888_SNORM,
		ClPixelFormat_RGBA4444_UNORM_PACKED,
		ClPixelFormat_BGRA4444_SNORM_PACKED,
		ClPixelFormat_RGB565_UNORM_PACKED,
		ClPixelFormat_Depth_24F,
		ClPixelFormat_Depth32F,
		ClPixelFormat_Stencil8,
		ClPixelFormat_Depth24Stencil8_PACKED,
		ClPixelFormat_Depth32Stencil8,
	};

	enum ClVertexFormat:uint8
	{
		ClVertexFormatInvalid = 0,
		ClVertexFormatByte,
		ClVertexFormatByte2,
		ClVertexFormatByte3,
		ClVertexFormatByte4,
		ClVertexFormatByteNorm,
		ClVertexFormatByte2Norm,
		ClVertexFormatByte3Norm,
		ClVertexFormatByte4Norm,
		ClVertexFormatUShort,
		ClVertexFormatUShort2,
		ClVertexFormatUShort3,
		ClVertexFormatUShort4,
		ClVertexFormatUShortNorm,
		ClVertexFormatUShort2Norm,
		ClVertexFormatUShort3Norm,
		ClVertexFormatUShort4Norm,
		ClVertexFormatUInt,
		ClVertexFormatUInt2,
		ClVertexFormatUInt3,
		ClVertexFormatUInt4,
		ClVertexFormatUIntNorm,
		ClVertexFormatUInt2Norm,
		ClVertexFormatUInt3Norm,
		ClVertexFormatUInt4Norm,
		ClVertexFormatShort,
		ClVertexFormatShort2,
		ClVertexFormatShort3,
		ClVertexFormatShort4,
		ClVertexFormatShortNorm,
		ClVertexFormatShort2Norm,
		ClVertexFormatShort3Norm,
		ClVertexFormatShort4Norm,
		ClVertexFormatInt,
		ClVertexFormatInt2,
		ClVertexFormatInt3,
		ClVertexFormatInt4,
		ClVertexFormatFloat,
		ClVertexFormatFloat2,
		ClVertexFormatFloat3,
		ClVertexFormatFloat4,
		ClVertexFormatFloatNorm,
		ClVertexFormatFloat2Norm,
		ClVertexFormatFloat3Norm,
		ClVertexFormatFloat4Norm,
	};

	enum ClVertexStepFunction :uint8
	{
		ClInvalidStepFunction,
		ClStepPerVertex,
		ClStepPerInstance,
		ClStepConstant,
	};

	enum ClCompareFunction :uint8
	{
		ClCompareAlways,
		ClCompareNever,
		ClCompareLess,
		ClCompareLessEqual,
		ClCompareEqual,
		ClCompareGreaterEqual,
		ClCompareGreater
	};

	enum ClWriteMask :uint8
	{
		ClWriteMaskNone,
		ClWriteMaskRed = 0x1,
		ClWriteMaskGreen = 0x2,
		ClWriteMaskBlue = 0x4,
		ClWriteMaskAlpha = 0x8,
		ClWriteMaskAll = ClWriteMaskRed | ClWriteMaskGreen | ClWriteMaskBlue | ClWriteMaskAlpha
	};

	enum ClCullMode :uint8
	{
		ClCullNone,
		ClCullBack,
		ClCullFront,
	};

	enum ClWindingMode :uint8
	{
		ClWindingCW = 0,// Clockwise winding
		ClWindingCCW, // Counter clockwise winding
		ClWindingDefault = ClWindingCW,
	};

	enum ClBlendOperation
	{
		ClBlendOperationAdd,
		ClBlendOperationSubtract,
		ClBlendOperationReverseSubtract,
		ClBlendOperationMin,
		ClBlendOperationMax
	};

	enum ClStencilOperator
	{
		ClStencilOpKeep,
		ClStencilOpZero,
		ClStencilOpReplace,
		ClStencilOpInc,
		ClStencilOpDec,
		ClStencilInvert
	};

	enum ClPolygonMode
	{
		ClPolygonModePoint,
		ClPolygonModeWireframe,
		ClPolygonModeFill
	};

	enum ClBlendFactor
	{
		ClBlendFactorZero,
		ClBlendFactorOne,
		ClBlendFactorSourceColor,
		ClBlendFactorSourceAlpha,
		ClBlendFactorDestinationColor,
		ClBlendFactorDestinationAlpha,
		ClBlendFactorOneMinusSourceColor,
		ClBlendFactorOneMinusSourceAlpha,
		ClBlendFactorOneMinusDestinationColor,
		ClBlendFactorOneMinusDestinationAlpha,
		ClBlendFactorSourceAlphaMinor,
		ClBlendFactorColor,
		ClBlendFactorOneMinusColor,
		ClBlendFactorAlpha,
		ClBlendFactorOneMinusAlpha,
		ClBlendFactorSource1Color,
		ClBlendFactorOneMinusSource1Color,
		ClBlendFactorSource1Alpha,
		ClBlendFactorOneMinusSource1Alpha,
	};

	enum ClTextureUsage:uint8
	{
		/*
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT = 0x00000001,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT = 0x00000002,
		VK_IMAGE_USAGE_SAMPLED_BIT = 0x00000004,
		VK_IMAGE_USAGE_STORAGE_BIT = 0x00000008,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT = 0x00000010,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 0x00000020,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT = 0x00000040,
		VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT = 0x00000080,
		*/
		ClTextureUsageTransferSrc = 0x1,
		ClTextureUsageTransferDst = 0x2,
		ClTextureUsageSampled = 0x4,
		ClTextureUsageStorage = 0x8,
		ClTextureUsageColorAttachment = 0x10,
		ClTextureUsageDepthStencilAttachment = 0x20,
		ClTextureUsageTransientAttachment = 0x40,
		ClTextureUsageInputAttachment = 0x80,
	};

	enum ClTextureAddress
	{
		ClTextureAddressWrap,
		ClTextureAddressMirror,
		ClTextureAddressClamp,
		ClTextureAddressClampOne,
		ClTextureAddressClampZero
	};

	enum ClSamplerFilter
	{
		ClFilterLinear,
		ClFilterPoint,
		ClFilerNone,
	};

	enum ClTextureType
	{
		ClTexture1D,
		ClTexture2D,
		ClTexture3D,
		ClTextureCube
	};

	enum ClLoadAction
	{
		ClLoadActionLoad,
		ClLoadActionClear,
		ClLoadActionDontCare
	};

	enum ClStoreAction
	{
		ClStoreActionStore,
		ClStoreActionDontCare
	};

	struct ClTextureDesc
	{
		uint16 width = 0;
		uint16 height = 0;
		ClPixelFormat format = ClPixelFormat_Invalid;
		bool mipmap = false;
		//
		uint8 usage =
			  ClTextureUsageTransferSrc
			| ClTextureUsageTransferDst
			| ClTextureUsageSampled
			| ClTextureUsageStorage
			| ClTextureUsageColorAttachment;
	};

	struct ClVertexAttribute
	{
		ClVertexFormat format = ClVertexFormatInvalid;
		uint16 offset = 0;
		uint8 bufferIndex = 0xff;
	};

	struct ClVertexLayout
	{
		ClVertexStepFunction stepFunction = ClInvalidStepFunction;
		uint16 stepRate = 1;
		uint16 stride = 0;
	};

	struct ClVertexDesc
	{
		static const uint8 Cl_VERTEX_SUPPORT_MAX = 12;
		ClVertexAttribute attributes[Cl_VERTEX_SUPPORT_MAX];
		ClVertexLayout layouts[Cl_VERTEX_SUPPORT_MAX];
		uint8 nAttri = 0;
		uint8 nLayout = 0;
	};

	struct ClAttachmentDesc
	{
		static const uint8 COLOR_ATTACHMENT_MAX = 4;
		//
		uint8 colorAttachmentCount = 1;
		ClPixelFormat colorAttachments[COLOR_ATTACHMENT_MAX] = { ClPixelFormat_RGBA8888_UNORM };
		ClPixelFormat depthAttachment = ClPixelFormat_Depth32F;
		ClPixelFormat stencilAttachment = ClPixelFormat_Invalid;
	};

	struct ClDepthState
	{
		bool writable = true;
		bool testEnable = true;
		ClCompareFunction compareFunction = ClCompareLessEqual;
	};

	struct ClStencilState
	{
		//
		uint8 writeMask = 0xff;
		uint8 readMask = 0xff;
		//
		bool enable = false;
		ClStencilOperator fail = ClStencilOpKeep;
		ClStencilOperator zFail = ClStencilOpKeep;
		ClStencilOperator pass = ClStencilOpReplace;
		//
		bool ccwEnabled = false;
		ClStencilOperator ccwFail = ClStencilOpKeep;
		ClStencilOperator ccwZFail = ClStencilOpKeep;
		ClStencilOperator ccwPass = ClStencilOpReplace;
	};

	struct ClBlendState
	{
		bool blendEnabled = false;
		ClBlendOperation alphaBlending;
		ClBlendOperation rgbBlending;
		ClBlendFactor sourceRgbBlendFactor;
		ClBlendFactor destinationRgbBlendFactor;
		ClBlendFactor sourceAlphaBlendFactor;
		ClBlendFactor destinationAlphaBlendFactor;
	};

	struct ClRenderState
	{
		ClPolygonMode polygonMode = ClPolygonModeFill;
		ClCullMode cullMode = ClCullNone;
		ClWindingMode winding = ClWindingCW;
		ClWriteMask writeMask = ClWriteMaskAll;
		// depth
		ClDepthState depthState;
		// stencil
		ClStencilState stencilState;
		// blending
		ClBlendState blendState;
	};

	struct ClPipelineDesc
	{
		ClRenderState renderState;
		//
		ClVertexDesc vertexDesc;
		const char * vertexShader;
		const char * fragmentShader;
		//
		ClAttachmentDesc attachmentDesc;
	};

	struct ClSamplerDesc
	{
		ClTextureAddress U;
		ClTextureAddress V;
		ClTextureAddress W;
		ClSamplerFilter minFilter;
		ClSamplerFilter magFilter;
		ClSamplerFilter mipFilter;
	};

	template <class T>
	struct ClRect
	{
		T left, right, bottom, top;
		ClRect()
		{
			left = right = bottom = top = 0;
		}
	};
};

