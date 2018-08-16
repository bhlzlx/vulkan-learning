#include "Framebuffer.h"

namespace clannad
{
	namespace vulkan
	{
		VkRenderPass CreateRenderPass(VkDevice _device, const ClAttachmentDesc& _attachmentDesc, ClLoadAction _loadAction, ClStoreAction _storeAction)
		{
			struct AttachmentStruct
			{
				VkAttachmentReference references[ClAttachmentDesc::COLOR_ATTACHMENT_MAX + 2];
				VkAttachmentDescription attachments[ClAttachmentDesc::COLOR_ATTACHMENT_MAX + 2];
			} attachmentStruct;

			uint8 attachmentCount = 0;
			for (attachmentCount = 0; attachmentCount < _attachmentDesc.colorAttachmentCount + 2; ++attachmentCount)
			{
				attachmentStruct.attachments[attachmentCount].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachmentStruct.attachments[attachmentCount].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachmentStruct.attachments[attachmentCount].flags = 0;
				attachmentStruct.attachments[attachmentCount].samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentStruct.attachments[attachmentCount].loadOp = mappingLoadAction(_loadAction);
				attachmentStruct.attachments[attachmentCount].storeOp = mappingStoreAction(_storeAction);
				attachmentStruct.attachments[attachmentCount].stencilLoadOp = mappingLoadAction(_loadAction);
				attachmentStruct.attachments[attachmentCount].stencilStoreOp = mappingStoreAction(_storeAction);
				//
				attachmentStruct.references[attachmentCount].attachment = attachmentCount;
				attachmentStruct.references[attachmentCount].layout = VK_IMAGE_LAYOUT_GENERAL;
			}

			for (attachmentCount = 0; attachmentCount < _attachmentDesc.colorAttachmentCount; ++attachmentCount)
			{
				attachmentStruct.attachments[attachmentCount].format = mappingFormat(_attachmentDesc.colorAttachments[attachmentCount]);
			}
			if (_attachmentDesc.depthAttachment)
			{
				attachmentStruct.attachments[attachmentCount].format = mappingFormat(_attachmentDesc.depthAttachment);
				++attachmentCount;
			}
			if (_attachmentDesc.stencilAttachment)
			{
				attachmentStruct.attachments[attachmentCount].format = mappingFormat(_attachmentDesc.stencilAttachment);
				++attachmentCount;
			}

			VkSubpassDependency dependency;
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			typedef struct VkAttachmentReference {
				uint32_t         attachment;
				VkImageLayout    layout;
			} VkAttachmentReference;

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = _attachmentDesc.colorAttachmentCount;
			subpass.pColorAttachments = &attachmentStruct.references[0];
			if (_attachmentDesc.depthAttachment)
				subpass.pDepthStencilAttachment = &attachmentStruct.references[_attachmentDesc.colorAttachmentCount];
			else
				subpass.pDepthStencilAttachment = nullptr;

			VkRenderPassCreateInfo rpInfo;
			rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			rpInfo.flags = 0;
			rpInfo.attachmentCount = attachmentCount;
			rpInfo.pAttachments = &attachmentStruct.attachments[0];
			rpInfo.pNext = nullptr;
			rpInfo.pDependencies = &dependency;
			rpInfo.dependencyCount = 1;
			rpInfo.pNext = nullptr;
			rpInfo.subpassCount = 1;
			rpInfo.pSubpasses = &subpass;

			VkRenderPass renderPass;
			vkCreateRenderPass(_device, &rpInfo, nullptr, &renderPass);

			return renderPass;
		}

		Framebuffer::Framebuffer()
		{
		}

		Framebuffer::~Framebuffer()
		{
		}

		// 
		void Framebuffer::match(uint32_t _width, uint32_t _height)
		{
			if (this->_width == _width && this->_height == _height)
			{
				return;
			}



		}

		Framebuffer* Framebuffer::Create( Device* _host, const ClAttachmentDesc& _attachmentDesc)
		{
			Framebuffer * framebuffer = new Framebuffer;
			framebuffer->_host = _host;
			framebuffer->_attachmentDesc = _attachmentDesc;
			return framebuffer;
		}

	}
}




