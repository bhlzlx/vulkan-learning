#include "PipelineVk.h"
#include "DeviceVk.h"
#include "vkTypeMapping.h"


namespace clannad
{
	namespace vulkan
	{
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



