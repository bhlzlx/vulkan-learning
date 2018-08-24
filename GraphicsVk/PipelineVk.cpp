#include "PipelineVk.h"
#include "DeviceVk.h"
#include "vkTypeMapping.h"


namespace clannad
{
	namespace vulkan
	{
		Pipeline* Pipeline::Create(Device* _host, const ClPipelineDesc& _pipelineDesc)
		{
			VkDynamicState;
			VertexInputState vertexInputState;
			GenerateVertexInputStateInfo(_pipelineDesc.vertexLayout, vertexInputState);
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
			GenerateInputAssemblyStateInfo(_pipelineDesc.drawMode, inputAssemblyCreateInfo);
			VkPipelineRasterizationStateCreateInfo rasterStateInfo;
			GenerateRasterizationStateInfo( _pipelineDesc, rasterStateInfo);
			VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo;
			GenerateDepthStencilStateInfo(_pipelineDesc.renderState, depthStencilCreateInfo);
			VkPipelineColorBlendStateCreateInfo blendStateCreateInfo;
			GenerateBlendStateInfo(_pipelineDesc.renderState,blendStateCreateInfo);
			//
			VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
				//VK_DYNAMIC_STATE_LINE_WIDTH,
				//VK_DYNAMIC_STATE_DEPTH_BIAS,
				VK_DYNAMIC_STATE_BLEND_CONSTANTS,
				VK_DYNAMIC_STATE_DEPTH_BOUNDS,
				//VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
				//VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
				VK_DYNAMIC_STATE_STENCIL_REFERENCE,
				//VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV,
				//VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT,
				//VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT,
			};
			dynamicStateCreateInfo.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
			dynamicStateCreateInfo.flags = 0;
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.pNext = nullptr;
			//
			VkPipelineMultisampleStateCreateInfo multisample_state_create_info =
			{ 
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				nullptr,
				0,
				VK_SAMPLE_COUNT_1_BIT, //sample_count,
				false, //per_sample_shading_enable,
				1, //min_sample_shading,
				nullptr, //sample_masks,
				true,//alpha_to_coverage_enable,
				false //alpha_to_one_enable 
			};

			//
			VkPipelineCacheCreateInfo pipelineInfo;
			pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
			pipelineInfo.flags = 0;
			pipelineInfo.pNext = nullptr;
			//pipelineInfo.

			VkRect2D scissor;
			VkViewport viewport;
			VkPipelineViewportStateCreateInfo viewportStateInfo;
			viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateInfo.flags = 0;
			viewportStateInfo.pNext = nullptr;
			viewportStateInfo.pScissors = &scissor;
			viewportStateInfo.scissorCount = 1;
			viewportStateInfo.pViewports = &viewport;
			viewportStateInfo.viewportCount = 1;
			//vkCreateGraphicsPipelines()



			return nullptr;
		}


		void Pipeline::GenerateVertexInputStateInfo(const ClVertexLayout& _vertexLayout, VertexInputState& _vertexInputState)
		{

			for (size_t i = 0; i < _vertexLayout.attributeCount; ++i)
			{
				_vertexInputState.attributes[i].binding = _vertexLayout.attributes[i].inputIndex;
				_vertexInputState.attributes[i].format = mappingVertexFormat(_vertexLayout.attributes[i].format);
				_vertexInputState.attributes[i].location = i;
			}

			for (size_t i = 0; i < _vertexLayout.inputCount; ++i)
			{
				_vertexInputState.bindings[i].binding = i;
				_vertexInputState.bindings[i].stride = _vertexLayout.inputs[i].stride;
				_vertexInputState.bindings[i].inputRate = _vertexLayout.inputs[i].stepFunction == ClStepPerVertex ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
			}

			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
			vertexInputCreateInfo.flags = 0;
			vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputCreateInfo.pNext = nullptr;
			vertexInputCreateInfo.pVertexAttributeDescriptions = &_vertexInputState.attributes[0];
			vertexInputCreateInfo.vertexAttributeDescriptionCount = _vertexLayout.attributeCount;
			vertexInputCreateInfo.pVertexBindingDescriptions = &_vertexInputState.bindings[0];
			vertexInputCreateInfo.vertexBindingDescriptionCount = _vertexLayout.inputCount;
		}


		void Pipeline::GenerateInputAssemblyStateInfo(const ClDrawMode& _drawMode, VkPipelineInputAssemblyStateCreateInfo& _inputAssemblyState)
		{
			_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			_inputAssemblyState.flags = 0;
			_inputAssemblyState.pNext = nullptr;
			_inputAssemblyState.primitiveRestartEnable = false;
			_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}

		void Pipeline::GenerateRasterizationStateInfo(const ClPipelineDesc& _desc, VkPipelineRasterizationStateCreateInfo& _rasterState)
		{
			ClPolygonMode polygonMode = _desc.drawMode.polygonMode;
			VkFrontFace frontFace = _desc.drawMode.frontFace == ClClockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;

			_rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			_rasterState.pNext = nullptr;
			_rasterState.flags = 0;
			_rasterState.cullMode = mappintCullMode(_desc.renderState.cullMode);
			_rasterState.polygonMode = mappingPolygonMode(polygonMode);
			_rasterState.depthBiasClamp = 0;
			_rasterState.depthBiasEnable = false;
			_rasterState.depthBiasConstantFactor = 0.0f;
			_rasterState.depthBiasSlopeFactor = 0.0f;
			_rasterState.depthClampEnable = false;
			_rasterState.frontFace = frontFace;
			_rasterState.lineWidth = 1.0f;
			_rasterState.rasterizerDiscardEnable = true;
		}


		void Pipeline::GenerateDepthStencilStateInfo(const ClRenderState& _renderState, VkPipelineDepthStencilStateCreateInfo& _depthStencilState)
		{
			_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			_depthStencilState.flags = 0;
			_depthStencilState.pNext = nullptr;
			_depthStencilState.depthBoundsTestEnable = _renderState.depthState.testEnable;
			_depthStencilState.depthCompareOp = mappingCompareFunction(_renderState.depthState.compareFunction);
			_depthStencilState.depthWriteEnable = _renderState.depthState.writable;
			_depthStencilState.depthBoundsTestEnable = true;
			_depthStencilState.stencilTestEnable = _renderState.stencilState.enable;
			VkStencilOpState opState;
			opState.compareMask = _renderState.stencilState.readMask;
			opState.writeMask = _renderState.stencilState.writeMask;
			opState.compareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			opState.passOp = mappingStencilOp(_renderState.stencilState.pass);
			opState.failOp = mappingStencilOp(_renderState.stencilState.fail);
			opState.depthFailOp = VK_STENCIL_OP_KEEP;
			opState.reference = 0.0f;

			_depthStencilState.front = opState;
			opState.passOp = mappingStencilOp(_renderState.stencilState.ccwPass);
			opState.failOp = mappingStencilOp(_renderState.stencilState.ccwFail);
			_depthStencilState.back = opState;
		}


		void Pipeline::GenerateBlendStateInfo(const ClRenderState& _renderState, VkPipelineColorBlendStateCreateInfo& _blendInfo)
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			VkColorComponentFlags colorMask = 0;
			if (_renderState.writeMask & ClWriteMaskRed)
				colorMask |= VK_COLOR_COMPONENT_R_BIT;
			if (_renderState.writeMask & ClWriteMaskGreen)
				colorMask |= VK_COLOR_COMPONENT_G_BIT;
			if (_renderState.writeMask & ClWriteMaskBlue)
				colorMask |= VK_COLOR_COMPONENT_B_BIT;
			if (_renderState.writeMask & ClWriteMaskAlpha)
				colorMask |= VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.colorWriteMask = colorMask;
			//
			colorBlendAttachment.blendEnable = _renderState.blendState.blendEnabled;
			colorBlendAttachment.srcColorBlendFactor = mappingBlendFactor( _renderState.blendState.sourceRgbBlendFactor ); // Optional
			colorBlendAttachment.dstColorBlendFactor = mappingBlendFactor(_renderState.blendState.destinationRgbBlendFactor); // Optional
			colorBlendAttachment.colorBlendOp = mappingBlendOp(_renderState.blendState.rgbBlending); // Optional
			colorBlendAttachment.srcAlphaBlendFactor = mappingBlendFactor(_renderState.blendState.sourceAlphaBlendFactor); // Optional
			colorBlendAttachment.dstAlphaBlendFactor = mappingBlendFactor(_renderState.blendState.destinationAlphaBlendFactor); // Optional
			colorBlendAttachment.alphaBlendOp = mappingBlendOp(_renderState.blendState.alphaBlending); // Optional
		}

		Pipeline::Pipeline()
		{
		}


		Pipeline::~Pipeline()
		{
		}
	}


}



