#pragma once
#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"
#include "clGraphic.h"

namespace clannad
{
	namespace vulkan
	{
		class Device;
		class Pipeline
		{
		private:
			VkViewport _viewport;
			VkRect2D _scissor;

			VkVertexInputBindingDescription vertexInputsDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];
			VkVertexInputAttributeDescription vertexAttriDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];

		public:
			Pipeline();
			~Pipeline();
		public:
			static Pipeline* Create(Device* _host, const ClPipelineDesc& _pipelineDesc);

			struct VertexInputState
			{
				VkPipelineVertexInputStateCreateInfo data;
				VkVertexInputBindingDescription bindings[ClVertexLayout::VERTEX_SUPPORT_MAX];
				VkVertexInputAttributeDescription attributes[ClVertexLayout::VERTEX_SUPPORT_MAX];

				VertexInputState(){
				}

				VertexInputState(const VertexInputState& _from)
				{
					*this = _from;
					data.pVertexAttributeDescriptions = &attributes[0];
					data.pVertexBindingDescriptions = &bindings[0];
				}
			};
			static void GenerateVertexInputStateInfo( const ClVertexLayout& _vertexLayout, VertexInputState& _vertexInputState);
			static void GenerateInputAssemblyStateInfo(const ClDrawMode& _drawMode, VkPipelineInputAssemblyStateCreateInfo& _inputAssemblyState);
			static void GenerateRasterizationStateInfo(const ClPipelineDesc& _desc, VkPipelineRasterizationStateCreateInfo& _rasterState);
			static void GenerateDepthStencilStateInfo(const ClRenderState& _depthStencilDesc, VkPipelineDepthStencilStateCreateInfo& _depthStencilState);
			static void GenerateBlendStateInfo(const ClRenderState& _blendState, VkPipelineColorBlendStateCreateInfo& _blendInfo);
		};

	}
}


