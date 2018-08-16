#include "FramebufferVk.h"
#include "DeviceVk.h"
#include "TextureVk.h"
#include "VkApi.h"

namespace clannad
{
	namespace vulkan
	{
		VkRenderPass CreateRenderPass(VkDevice _device, const ClAttachmentDesc& _attachmentDesc, ClLoadAction _loadAction, ClStoreAction _storeAction)
		{
			// 这个暂时只创建一个 render pass, 即 render pass 只有一个 sub pass
			struct AttachmentStruct
			{
				VkAttachmentReference references[ClAttachmentDesc::COLOR_ATTACHMENT_MAX + 1];
				VkAttachmentDescription attachments[ClAttachmentDesc::COLOR_ATTACHMENT_MAX + 1];
			} attachmentStruct;

			uint8 attachmentIndex = 0;
			for (attachmentIndex = 0; attachmentIndex < _attachmentDesc.colorAttachmentCount + 1; ++attachmentIndex)
			{
				attachmentStruct.attachments[attachmentIndex].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachmentStruct.attachments[attachmentIndex].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachmentStruct.attachments[attachmentIndex].flags = 0;
				attachmentStruct.attachments[attachmentIndex].samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentStruct.attachments[attachmentIndex].loadOp = mappingLoadAction(_loadAction);
				attachmentStruct.attachments[attachmentIndex].storeOp = mappingStoreAction(_storeAction);
				attachmentStruct.attachments[attachmentIndex].stencilLoadOp = mappingLoadAction(_loadAction);
				attachmentStruct.attachments[attachmentIndex].stencilStoreOp = mappingStoreAction(_storeAction);
				//
				attachmentStruct.references[attachmentIndex].attachment = attachmentIndex;
				attachmentStruct.references[attachmentIndex].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}

			for (attachmentIndex = 0; attachmentIndex < _attachmentDesc.colorAttachmentCount; ++attachmentIndex)
			{
				attachmentStruct.attachments[attachmentIndex].format = mappingFormat(_attachmentDesc.colorAttachments[attachmentIndex]);
			}
			if (_attachmentDesc.depthStencilAttachment)
			{
				attachmentStruct.attachments[attachmentIndex].format = mappingFormat(_attachmentDesc.depthStencilAttachment);
				//
				attachmentStruct.references[attachmentIndex].attachment = attachmentIndex;
				attachmentStruct.references[attachmentIndex].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				++attachmentIndex;
			}

			VkSubpassDependency dependency;
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = _attachmentDesc.colorAttachmentCount;
			subpass.pColorAttachments = &attachmentStruct.references[0];
			if (_attachmentDesc.depthStencilAttachment)
				subpass.pDepthStencilAttachment = &attachmentStruct.references[_attachmentDesc.colorAttachmentCount];
			else
				subpass.pDepthStencilAttachment = nullptr;

			VkRenderPassCreateInfo rpInfo;
			rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			rpInfo.flags = 0;
			rpInfo.attachmentCount = attachmentIndex;
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

		void Framebuffer::_clear()
		{
			for ( auto & tex : _colorAttachments)
			{
				if ( tex)
				{
					tex->release();
					tex = nullptr;
				}
			}
			if (_depthStencilAttachment)
			{
				_depthStencilAttachment->release();
				_depthStencilAttachment = nullptr;
			}
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
			_clear();
			// recreate
			VkImageView imageViews[ClAttachmentDesc::COLOR_ATTACHMENT_MAX + 1];
			size_t i = 0;
			for (; i < _attachmentDesc.colorAttachmentCount; ++i)
			{
				ClTextureDesc desc;
				desc.format = _attachmentDesc.colorAttachments[i];
				desc.mipmap = false;
				desc.width = _width;
				desc.height = _height;
				_colorAttachments[i] = Texture2D::Create(_host, desc);
				imageViews[i] = _colorAttachments[i]->getImageView();
			}
			if (_attachmentDesc.depthStencilAttachment != ClPixelFormat_Invalid)
			{
				ClTextureDesc desc;
				desc.format = _attachmentDesc.depthStencilAttachment;
				desc.mipmap = false;
				desc.width = _width;
				desc.height = _height;
				_depthStencilAttachment = Texture2D::Create(_host, desc);
				imageViews[i] = _depthStencilAttachment->getImageView();
			}
			//
			VkFramebufferCreateInfo framebufferInfo;
			framebufferInfo.attachmentCount = _attachmentDesc.depthStencilAttachment ? _attachmentDesc.colorAttachmentCount + 1 : _attachmentDesc.colorAttachmentCount;
			framebufferInfo.flags = 0;
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.width = _width;
			framebufferInfo.height = _height;
			framebufferInfo.layers = 1;
			framebufferInfo.pNext = nullptr;
			framebufferInfo.renderPass = _renderPass;
			framebufferInfo.pAttachments = &imageViews[0];
			//
			vkCreateFramebuffer(*_host, &framebufferInfo, NULL, &_id);
		}

		Framebuffer* Framebuffer::Create( Device* _host, const ClAttachmentDesc& _attachmentDesc)
		{
			Framebuffer * framebuffer = new Framebuffer;
			framebuffer->_host = _host;
			framebuffer->_attachmentDesc = _attachmentDesc;
			framebuffer->_renderPass = CreateRenderPass( *_host, _attachmentDesc);
			return framebuffer;
		}

	}
}




