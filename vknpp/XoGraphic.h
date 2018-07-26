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

	enum XoPixelFormat
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
		XoPixelFormat_Depth24Stencil8,
		XoPixelFormat_Depth32Stencil8,
	};

	enum XoVertexFormat
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
		XoVertexFormatFloat,
		XoVertexFormatFloat2,
		XoVertexFormatFloat3,
		XoVertexFormatFloat4,
		XoVertexFormatFloatNorm,
		XoVertexFormatFloat2Norm,
		XoVertexFormatFloat3Norm,
		XoVertexFormatFloat4Norm,
	};

	enum XoVertexStepFunction
	{
		XoInvalidStepFunction,
		XoStepPerVertex,
		XoStepPerInstance,
		XoStepConstant,
	};

	enum XoWriteMask
	{
		XoWriteMaskNone,
		XoWriteMaskRed = 0x1,
		XoWriteMaskGreen = 0x2,
		XoWriteMaskBlue = 0x4,
		XoWriteMaskAlpha = 0x8,
		XoWriteMaskAll = XoWriteMaskRed | XoWriteMaskGreen | XoWriteMaskBlue | XoWriteMaskAlpha
	};

	enum XoBlendOperation
	{
		XoBlendOperationAdd,
		XoBlendOperationSubtract,
		XoBlendOperationReverseSubtract,
		XoBlendOperationMin,
		XoBlendOperationMax
	};

	enum XoBlendFactor
	{
		XoBlendFactorZero,
		XoBlendFactorOne,
		XoBlendFactorSourceColor,
		XoBlendFactorSourceAlpha,
		//...
	};

	struct XoVertexAttribute
	{
		XoVertexFormat format;
		uint16 offset;
		uint8 bufferIndex;
	};

	struct XoVertexLayout
	{
		XoVertexStepFunction stepFunction;
		uint16 stepRate;
		uint16 stride;
	};

	struct XoVertexDesc
	{
		static const uint8 XO_VERTEX_SUPPORT_MAX = 12;
		XoVertexAttribute attributes[XO_VERTEX_SUPPORT_MAX];
		XoVertexLayout layouts[XO_VERTEX_SUPPORT_MAX];
		uint8 nAttri;
		uint8 nLayout;
	};

	struct XoRenderPassAttachmentDesc
	{
		static const uint8 XO_COLOR_ATTACHMENT_MAX = 4;
		XoPixelFormat colorAttachments[XO_COLOR_ATTACHMENT_MAX] = { XoPixelFormat_RGBA8888_UNORM };
		uint8 nColorAttachment = 1;
		XoPixelFormat depthAttachment = XoPixelFormat_Depth32Stencil8;
		XoPixelFormat stencilAttachment = XoPixelFormat_Depth32Stencil8;
	};

	struct XoRenderState
	{
		XoWriteMask writeMask;
		bool blendEnabled;
		XoBlendOperation alphaBlending;
		XoBlendOperation rgbBlending;
		XoBlendFactor sourceRgbBlendFactor;
		XoBlendFactor destinationRgbBlendFactor;
		XoBlendFactor sourceAlphaBlendFactor;
		XoBlendFactor destinationAlphaBlendFactor;
	};

	struct XoPipelineDesc
	{
		std::string vertexShader;
		std::string fragmentShader;
		//
		XoVertexDesc vertexDesc;
		//
		XoRenderPassAttachmentDesc attachments;
	};
};

