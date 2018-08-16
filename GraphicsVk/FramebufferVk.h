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
		class Texture2D;

		VkRenderPass CreateRenderPass(VkDevice _device, const ClAttachmentDesc& _attachmentDesc, ClLoadAction _loadAction = ClLoadActionDontCare, ClStoreAction _storeAction = ClStoreActionDontCare);

		VkAttachmentLoadOp mappingLoadAction(ClLoadAction _loadOp)
		{
			switch (_loadOp)
			{
			case clannad::ClLoadActionLoad:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case clannad::ClLoadActionClear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case clannad::ClLoadActionDontCare:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:
				break;
			}
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}

		VkAttachmentStoreOp mappingStoreAction(ClStoreAction _storeOp)
		{
			switch (_storeOp)
			{
			case clannad::ClStoreActionStore:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case clannad::ClStoreActionDontCare:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			default:
				break;
			}
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}

		VkFormat mappingFormat(ClPixelFormat _format);

		

		class Framebuffer
		{
		private:
			ClAttachmentDesc _attachmentDesc;
			//
			VkFramebuffer _id;
			VkRenderPass _renderPass = nullptr;
			//
			Texture2D* _colorAttachments[ClAttachmentDesc::COLOR_ATTACHMENT_MAX] = { nullptr };
			Texture2D* _depthStencilAttachment = nullptr;
			//
			uint32_t _width = 0;
			uint32_t _height = 0;
			//
			Device* _host = nullptr;
			//
		private:
			void _clear();
		public:
			Framebuffer();
			~Framebuffer();
			void match(uint32_t _width, uint32_t _height);
		public:
			static Framebuffer* Create( Device* _host, const ClAttachmentDesc& _attachmentDesc);
		};
	}
}



