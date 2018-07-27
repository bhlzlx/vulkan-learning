#pragma once
#include <stdint.h>
#include <string>

namespace Xo
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

	enum XoPixelFormat:uint8
	{
		XoPixelFormat_Invalid = 0,
		XoPixelFormat_RGBA8888_UNORM,
		XoPixelFormat_RGBA8888_SNORM,
		XoPixelFormat_RGBA4444_UNORM_PACKED,
		XoPixelFormat_BGRA4444_SNORM_PACKED,
		XoPixelFormat_RGB565_UNORM_PACKED,
		XoPixelFormat_Depth_24F,
		XoPixelFormat_Depth_32F,
		XoPixelFormat_Stencil8,
		XoPixelFormat_Depth24Stencil8_PACKED,
		XoPixelFormat_Depth32Stencil8,
	};

	enum XoVertexFormat:uint8
	{
		XoVertexFormatInvalid = 0,
		XoVertexFormatByte,
		XoVertexFormatByte2,
		XoVertexFormatByte3,
		XoVertexFormatByte4,
		XoVertexFormatByteNorm,
		XoVertexFormatByte2Norm,
		XoVertexFormatByte3Norm,
		XoVertexFormatByte4Norm,
		XoVertexFormatUShort,
		XoVertexFormatUShort2,
		XoVertexFormatUShort3,
		XoVertexFormatUShort4,
		XoVertexFormatUShortNorm,
		XoVertexFormatUShort2Norm,
		XoVertexFormatUShort3Norm,
		XoVertexFormatUShort4Norm,
		XoVertexFormatUInt,
		XoVertexFormatUInt2,
		XoVertexFormatUInt3,
		XoVertexFormatUInt4,
		XoVertexFormatUIntNorm,
		XoVertexFormatUInt2Norm,
		XoVertexFormatUInt3Norm,
		XoVertexFormatUInt4Norm,
		XoVertexFormatShort,
		XoVertexFormatShort2,
		XoVertexFormatShort3,
		XoVertexFormatShort4,
		XoVertexFormatShortNorm,
		XoVertexFormatShort2Norm,
		XoVertexFormatShort3Norm,
		XoVertexFormatShort4Norm,
		XoVertexFormatInt,
		XoVertexFormatInt2,
		XoVertexFormatInt3,
		XoVertexFormatInt4,
		XoVertexFormatFloat,
		XoVertexFormatFloat2,
		XoVertexFormatFloat3,
		XoVertexFormatFloat4,
		XoVertexFormatFloatNorm,
		XoVertexFormatFloat2Norm,
		XoVertexFormatFloat3Norm,
		XoVertexFormatFloat4Norm,
	};

	enum XoVertexStepFunction :uint8
	{
		XoInvalidStepFunction,
		XoStepPerVertex,
		XoStepPerInstance,
		XoStepConstant,
	};

	enum XoCompareFunction :uint8
	{
		XoCompareAlways,
		XoCompareNever,
		XoCompareLess,
		XoCompareLessEqual,
		XoCompareEqual,
		XoCompareGreaterEqual,
		XoCompareGreater
	};

	enum XoWriteMask :uint8
	{
		XoWriteMaskNone,
		XoWriteMaskRed = 0x1,
		XoWriteMaskGreen = 0x2,
		XoWriteMaskBlue = 0x4,
		XoWriteMaskAlpha = 0x8,
		XoWriteMaskAll = XoWriteMaskRed | XoWriteMaskGreen | XoWriteMaskBlue | XoWriteMaskAlpha
	};

	enum XoCullMode :uint8
	{
		XoCullNone,
		XoCullBack,
		XoCullFront,
	};

	enum XoWindingMode :uint8
	{
		XoWindingCW = 0,// Clockwise winding
		XoWindingCCW, // Counter clockwise winding
		XoWindingDefault = XoWindingCW,
	};

	enum XoBlendOperation
	{
		XoBlendOperationAdd,
		XoBlendOperationSubtract,
		XoBlendOperationReverseSubtract,
		XoBlendOperationMin,
		XoBlendOperationMax
	};

	enum XoStencilOperator
	{
		XoStencilOpKeep,
		XoStencilOpZero,
		XoStencilOpReplace,
		XoStencilOpInc,
		XoStencilOpDec,
		XoStencilInvert
	};

	enum XoPolygonMode
	{
		XoPolygonModePoint,
		XoPolygonModeWireframe,
		XoPolygonModeFill
	};

	enum XoBlendFactor
	{
		XoBlendFactorZero,
		XoBlendFactorOne,
		XoBlendFactorSourceColor,
		XoBlendFactorSourceAlpha,
		XoBlendFactorDestinationColor,
		XoBlendFactorDestinationAlpha,
		XoBlendFactorOneMinusSourceColor,
		XoBlendFactorOneMinusSourceAlpha,
		XoBlendFactorOneMinusDestinationColor,
		XoBlendFactorOneMinusDestinationAlpha,
		XoBlendFactorSourceAlphaMinor,
		XoBlendFactorColor,
		XoBlendFactorOneMinusColor,
		XoBlendFactorAlpha,
		XoBlendFactorOneMinusAlpha,
		XoBlendFactorSource1Color,
		XoBlendFactorOneMinusSource1Color,
		XoBlendFactorSource1Alpha,
		XoBlendFactorOneMinusSource1Alpha,
	};

	enum XoTextureAccess:uint8
	{
		XoTextureAccessRead = 0x1,
		XoTextureAccessWrite = 0x2,
		XoTextureAccessFramebuffer = 0x4
	};

	enum XoTextureAddress
	{
		XoTextureAddressWrap,
		XoTextureAddressMirror,
		XoTextureAddressClamp,
		XoTextureAddressClampOne,
		XoTextureAddressClampZero
	};

	enum XoSamplerFilter
	{
		XoFilterLinear,
		XoFilterPoint,
		XoFilerNone,
	};

	struct XoTextureDesc
	{
		uint16 width;
		uint16 height;
		XoPixelFormat format;
		bool mipmap;
		//
		XoTextureAccess access;
	};

	struct XoVertexAttribute
	{
		XoVertexFormat format = XoVertexFormatInvalid;
		uint16 offset = 0;
		uint8 bufferIndex = 0xff;
	};

	struct XoVertexLayout
	{
		XoVertexStepFunction stepFunction = XoInvalidStepFunction;
		uint16 stepRate = 1;
		uint16 stride = 0;
	};

	struct XoVertexDesc
	{
		static const uint8 XO_VERTEX_SUPPORT_MAX = 12;
		XoVertexAttribute attributes[XO_VERTEX_SUPPORT_MAX];
		XoVertexLayout layouts[XO_VERTEX_SUPPORT_MAX];
		uint8 nAttri = 0;
		uint8 nLayout = 0;
	};

	struct XoFramebufferAttachmentDesc
	{
		static const uint8 XO_COLOR_ATTACHMENT_MAX = 4;
		XoPixelFormat colorAttachments[XO_COLOR_ATTACHMENT_MAX] = { XoPixelFormat_RGBA8888_UNORM };
		uint8 nColorAttachment = 1;
		XoPixelFormat depthAttachment = XoPixelFormat_Depth32Stencil8;
		XoPixelFormat stencilAttachment = XoPixelFormat_Depth32Stencil8;
	};

	struct XoDepthState
	{
		bool writable = true;
		bool testEnable = true;
		XoCompareFunction compareFunction = XoCompareLessEqual;
	};

	struct XoStencilState
	{
		//
		uint8 writeMask = 0xff;
		uint8 readMask = 0xff;
		//
		bool enable = false;
		XoStencilOperator fail = XoStencilOpKeep;
		XoStencilOperator zFail = XoStencilOpKeep;
		XoStencilOperator pass = XoStencilOpReplace;
		//
		bool ccwEnabled = false;
		XoStencilOperator ccwFail = XoStencilOpKeep;
		XoStencilOperator ccwZFail = XoStencilOpKeep;
		XoStencilOperator ccwPass = XoStencilOpReplace;
	};

	struct XoBlendState
	{
		bool blendEnabled = false;
		XoBlendOperation alphaBlending;
		XoBlendOperation rgbBlending;
		XoBlendFactor sourceRgbBlendFactor;
		XoBlendFactor destinationRgbBlendFactor;
		XoBlendFactor sourceAlphaBlendFactor;
		XoBlendFactor destinationAlphaBlendFactor;
	};

	struct XoRenderState
	{
		XoPolygonMode polygonMode = XoPolygonModeFill;
		XoCullMode cullMode = XoCullNone;
		XoWindingMode winding = XoWindingCW;
		XoWriteMask writeMask = XoWriteMaskAll;
		// depth
		XoDepthState depthState;
		// stencil
		XoStencilState stencilState;
		// blending
		XoBlendState blendState;
	};

	struct XoRenderpassDesc
	{
		// shading
		std::string vertexShader;
		std::string fragmentShader;
		// input vertex buffer format 
		XoVertexDesc vertexDesc;
		// render state
		XoRenderState renderState;
		// framebuffer structure description
		XoFramebufferAttachmentDesc attachments;
	};

	struct XoSamplerDesc
	{
		XoTextureAddress U;
		XoTextureAddress V;
		XoTextureAddress W;
		XoSamplerFilter minFilter;
		XoSamplerFilter magFilter;
		XoSamplerFilter mipFilter;
	};
};

