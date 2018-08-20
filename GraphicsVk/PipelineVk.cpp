#include "PipelineVk.h"
#include "DeviceVk.h"
#include "vkTypeMapping.h"


namespace clannad
{
	namespace vulkan
	{
		Pipeline* Pipeline::Create(Device* _host, const ClPipelineDesc& _pipelineDesc)
		{
			const ClVertexLayout& vertexLayout = _pipelineDesc.vertexLayout;
			VkVertexInputBindingDescription vertexInputsDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];
			VkVertexInputAttributeDescription vertexAttriDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];

			for (size_t i = 0; i < vertexLayout.attributeCount; ++i)
			{
				vertexAttriDesc[i].binding = vertexLayout.attributes[i].inputIndex;
				vertexAttriDesc[i].format = mappingVertexFormat(vertexLayout.attributes[i].format);
				vertexAttriDesc[i].location = i;
			}

			for (size_t i = 0; i < vertexLayout.inputCount; ++i)
			{
				vertexInputsDesc[i].binding = i;
				vertexInputsDesc[i].stride = vertexLayout.inputs[i].stride;
				vertexInputsDesc[i].inputRate = vertexLayout.inputs[i].stepFunction == ClStepPerVertex ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
			}

			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
			vertexInputCreateInfo.flags = 0;
			vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputCreateInfo.pNext = nullptr;
			vertexInputCreateInfo.pVertexAttributeDescriptions = &vertexAttriDesc[0];
			vertexInputCreateInfo.vertexAttributeDescriptionCount = vertexLayout.attributeCount;
			vertexInputCreateInfo.pVertexBindingDescriptions = &vertexInputsDesc[0];
			vertexInputCreateInfo.vertexBindingDescriptionCount = vertexLayout.inputCount;

			VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
			inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyCreateInfo.flags = 0;
			inputAssemblyCreateInfo.pNext = nullptr;
			inputAssemblyCreateInfo.primitiveRestartEnable = true;
			inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
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

			ClPolygonMode polygonMode = ClPolygonModeFill;
			VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;

			/*
			VkPipelineRasterizationStateCreateInfo	rasterization_state_create_info	=	{			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,			nullptr,			0,			depth_clamp_enable,			rasterizer_discard_enable,			polygon_mode,			culling_mode,			front_face,			depth_bias_enable,			depth_bias_constant_factor,			depth_bias_clamp,			depth_bias_slope_factor,			line_width	};
			*/

			VkPipelineRasterizationStateCreateInfo rasterStateInfo;
			rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterStateInfo.pNext = nullptr;
			rasterStateInfo.flags = 0;
			rasterStateInfo.cullMode = mappintCullMode(_pipelineDesc.renderState.cullMode);
			rasterStateInfo.polygonMode = mappingPolygonMode(polygonMode);
			rasterStateInfo.depthBiasClamp = 0;
			rasterStateInfo.depthBiasEnable = false;
			rasterStateInfo.depthBiasConstantFactor = 0.0f;
			rasterStateInfo.depthBiasSlopeFactor = 0.0f;
			rasterStateInfo.depthClampEnable = false;
			rasterStateInfo.frontFace = frontFace;
			rasterStateInfo.lineWidth = 1.0f;
			rasterStateInfo.rasterizerDiscardEnable = true;
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

			VkPipelineDepthStencilStateCreateInfo	depth_and_stencil_state_create_info = 
			{ 
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				nullptr,
				0,
				_pipelineDesc.renderState.depthState.testEnable, //depth_test_enable,
				_pipelineDesc.renderState.depthState.writable, //depth_write_enable,
				mappingCompareFunction(_pipelineDesc.renderState.depthState.compareFunction),//depth_compare_op,
				true, //depth_bounds_test_enable,
				_pipelineDesc.renderState.stencilState.enable,
				_pipelineDesc.renderState.stencilState.
				VkStencilOpState//front_stencil_test_parameters,
				back_stencil_test_parameters,
				min_depth_bounds,
				max_depth_bounds 
			};


			//vkCreateGraphicsPipelines()

			return nullptr;
		}

		Pipeline::Pipeline()
		{
		}


		Pipeline::~Pipeline()
		{
		}
	}


}



