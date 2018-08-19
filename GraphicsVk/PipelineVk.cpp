#include "PipelineVk.h"
#include "DeviceVk.h"

namespace clannad
{
	namespace vulkan
	{

		VkFormat mappingVertexFormat(ClVertexFormat _format)
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

		Pipeline* Pipeline::Create(Device* _host, ClVertexLayout _vertLayout)
		{
			VkVertexInputBindingDescription vertexInputsDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];
			VkVertexInputAttributeDescription vertexAttriDesc[ClVertexLayout::VERTEX_SUPPORT_MAX];

			for (size_t i = 0; i < _vertLayout.attributeCount; ++i)
			{
				vertexAttriDesc[i].binding = _vertLayout.attributes[i].inputIndex;
				vertexAttriDesc[i].format = mappingVertexFormat(_vertLayout.attributes[i].format);
				vertexAttriDesc[i].location = i;
			}

			for (size_t i = 0; i < _vertLayout.inputCount; ++i)
			{
				vertexInputsDesc[i].binding = i;
				vertexInputsDesc[i].stride = _vertLayout.inputs[i].stride;
				vertexInputsDesc[i].inputRate = _vertLayout.inputs[i].stepFunction == ClStepPerVertex ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
			}

			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
			vertexInputCreateInfo.flags = 0;
			vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputCreateInfo.pNext = nullptr;
			vertexInputCreateInfo.pVertexAttributeDescriptions = &vertexAttriDesc[0];
			vertexInputCreateInfo.vertexAttributeDescriptionCount = _vertLayout.attributeCount;
			vertexInputCreateInfo.pVertexBindingDescriptions = &vertexInputsDesc[0];
			vertexInputCreateInfo.vertexBindingDescriptionCount = _vertLayout.inputCount;

			VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
			inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyCreateInfo.flags = 0;
			inputAssemblyCreateInfo.pNext = nullptr;
			inputAssemblyCreateInfo.primitiveRestartEnable = true;
			inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}
}

Pipeline::Pipeline()
{
}


Pipeline::~Pipeline()
{
}

