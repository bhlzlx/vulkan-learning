#include "ViewVk.h"
#include "DeviceVk.h"

namespace clannad
{
	namespace vulkan
	{
		ApiLoader* GetApiLoader()
		{
			static ApiLoader * apiLoader = nullptr;
			if (!apiLoader)
			{
				apiLoader = new ApiLoader();
			}
			bool rst = apiLoader->Initialize();
			if (!rst)
			{
				delete apiLoader;
				apiLoader = nullptr;
				return nullptr;
			}
			return apiLoader;
		}
		//
		const Instance& GetVkInstance()
		{
			static Instance instance;
			if (!instance._id)
			{
				auto apiLoader = GetApiLoader();
				bool rst = apiLoader->CreateInstance(instance);
			}
			return instance;
		}
		//
		bool  View::_createSemaphore()
		{
			VkSemaphoreCreateInfo semaphore_create_info = {
				VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,      // VkStructureType          sType
				nullptr,                                      // const void*              pNext
				0                                             // VkSemaphoreCreateFlags   flags
			};
			
			VkDevice &device = _host->_id;
			DeviceApi &api = _host->_api;

			if (!api.vkCreateSemaphore(device, &semaphore_create_info, nullptr, &_imageAvail) == VK_SUCCESS)
				return false;
			if (!api.vkCreateSemaphore(device, &semaphore_create_info, nullptr, &_cmdExecuted) == VK_SUCCESS)
				return false;
			return true;
		}

		bool View::_createCommandPool()
		{
			VkCommandPoolCreateInfo cmdPoolCI = {
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType              sType
				nullptr,                                        // const void*                  pNext
				VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,                                              // VkCommandPoolCreateFlags     flags
				_host->_graphicQueueFamily,                 // uint32_t                     queueFamilyIndex
			};

			DeviceApi& deviceApi = _host->_api;
			VkDevice device = _host->_id;

			if (deviceApi.vkCreateCommandPool(device, &cmdPoolCI, nullptr, &_commandPool) != VK_SUCCESS)
			{
				return false;
			}

			return true;
		}

		bool View::_updateSwapchain()
		{
			const Instance& inst = GetVkInstance();
			if (!inst)
				return nullptr;
			//
			VkDevice &device = _host->_id;
			DeviceApi &api = _host->_api;
			_host->vkDeviceWaitIdle( device );
			//
			_swapchainCreateInfo.oldSwapchain = _swapchain;
			if (0 == _swapchainCreateInfo.imageExtent.width || 0 == _swapchainCreateInfo.imageExtent.height)
			{
				return false;
			}
			VkSwapchainKHR oldSwapchain = _swapchain;
			if (api.vkCreateSwapchainKHR( device , &_swapchainCreateInfo, nullptr, &_swapchain) != VK_SUCCESS)
			{
				return false;
			}
			if (oldSwapchain != VK_NULL_HANDLE)
			{
				api.vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
			}
			// 获取swapchain上的纹理
			uint32_t nImage = 0;
			api.vkGetSwapchainImagesKHR(device, _swapchain, &nImage, nullptr);
			if (!nImage)
				return false;
			_vecImages.resize(nImage);
			api.vkGetSwapchainImagesKHR(device, _swapchain, &nImage, &_vecImages[0]);
			// 创建command buffers
			if (_vecCommandBuffer.size())
			{
				api.vkFreeCommandBuffers(device, _commandPool, (uint32_t)_vecCommandBuffer.size(), &_vecCommandBuffer[0]);
			}
			_vecCommandBuffer.resize(nImage);
			//
			VkCommandBufferAllocateInfo cmd_buffer_allocate_info =
			{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType              sType
				nullptr,                                        // const void*                  pNext
				_commandPool,                     // VkCommandPool                commandPool
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel         level
				nImage                                    // uint32_t                     bufferCount
			};
			if ( api.vkAllocateCommandBuffers( device, &cmd_buffer_allocate_info, &_vecCommandBuffer[0]) != VK_SUCCESS)
			{
				return false;
			}
			//
			_needUpdateSwapchain = false;
			return true;
		}
		//
		bool View::onResize(int _width, int _height)
		{
			this->_width = _width;
			this->_height = _height;
			_swapchainCreateInfo.imageExtent.width = _width;
			_swapchainCreateInfo.imageExtent.height = _height;
			_needUpdateSwapchain = true;
			return true;
		}

		bool View::end()
		{
			return true;
		}

		bool View::begin()
		{
			if (_needUpdateSwapchain)
			{
				_updateSwapchain();
			}
			//
			uint32_t availImageIndex;
			VkResult result = _host->_api.vkAcquireNextImageKHR( _host->_id, _swapchain, UINT64_MAX, _imageAvail, VK_NULL_HANDLE, &availImageIndex);
			switch (result)
			{
			case VK_SUCCESS:
			case VK_SUBOPTIMAL_KHR:
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				_needUpdateSwapchain = true;
				return false;
			default:
				return false;
			}
			//
			VkCommandBuffer& commandBuffer = _vecCommandBuffer[availImageIndex];
			VkImage& image = _vecImages[availImageIndex];
			//
			VkCommandBufferBeginInfo beginInfo;
			//
			beginInfo.pInheritanceInfo = nullptr;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginInfo.pNext = nullptr;
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			//
			VkClearColorValue clear_color = {
				{ 1.0f, 0.8f, 0.4f, 0.0f }
			};

			VkImageSubresourceRange image_subresource_range = {
				VK_IMAGE_ASPECT_COLOR_BIT,                    // VkImageAspectFlags                     aspectMask
				0,                                            // uint32_t                               baseMipLevel
				1,                                            // uint32_t                               levelCount
				0,                                            // uint32_t                               baseArrayLayer
				1                                             // uint32_t                               layerCount
			};

			VkImageMemoryBarrier barrier_from_present_to_clear = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               srcQueueFamilyIndex
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               dstQueueFamilyIndex
				image,                       // VkImage                                image
				image_subresource_range                     // VkImageSubresourceRange                subresourceRange
			};

			VkImageMemoryBarrier barrier_from_clear_to_present = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               srcQueueFamilyIndex
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               dstQueueFamilyIndex
				image,                       // VkImage                                image
				image_subresource_range                     // VkImageSubresourceRange                subresourceRange
			};



			_host->_api.vkBeginCommandBuffer( commandBuffer, &beginInfo);
			_host->_api.vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);

			_host->_api.vkCmdClearColorImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range);

			_host->_api.vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);

			if (_host->_api.vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			{
				//std::cout << "Could not record command buffers!" << std::endl;
				return false;
			}			

			VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
			VkSubmitInfo submit_info =
			{
				VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
				nullptr,                                      // const void                  *pNext
				1,                                            // uint32_t                     waitSemaphoreCount
				&_imageAvail,              // const VkSemaphore           *pWaitSemaphores
				&wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
				1,                                            // uint32_t                     commandBufferCount
				&commandBuffer,  // const VkCommandBuffer       *pCommandBuffers
				1,                                            // uint32_t                     signalSemaphoreCount
				&_cmdExecuted            // const VkSemaphore           *pSignalSemaphores
			};

			if (_host->_api.vkQueueSubmit( _presentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
				return false;
			}

			VkPresentInfoKHR present_info = {
				VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
				nullptr,                                      // const void                  *pNext
				1,                                            // uint32_t                     waitSemaphoreCount
				&_cmdExecuted,           // const VkSemaphore           *pWaitSemaphores
				1,                                            // uint32_t                     swapchainCount
				(VkSwapchainKHR*)&_swapchain,                            // const VkSwapchainKHR        *pSwapchains
				&availImageIndex,                                 // const uint32_t              *pImageIndices
				nullptr                                       // VkResult                    *pResults
			};
			result = _host->_api.vkQueuePresentKHR(_presentQueue, &present_info);

			switch (result) {
			case VK_SUCCESS:
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
			case VK_SUBOPTIMAL_KHR:
				return onResize(0, 0);
			default:
				//std::cout << "Problem occurred during image presentation!" << std::endl;
				return false;
			}
			return true;
		}

		//
		bool View::prepareForSwapchain()
		{
			if (!_host)
				return false;
			// get command queues
			VkDevice &device = _host->_id;
			DeviceApi &api = _host->_api;
			//
			const Instance& inst = GetVkInstance();
			// 实际上在创建逻辑设备的时候这些队列都已经生成好，这里仅仅里取出来句柄
			api.vkGetDeviceQueue(device, _host->_graphicQueueFamily, 0, &_graphicQueue);
			api.vkGetDeviceQueue(device, _host->_presentQueueFamily, 0, &_presentQueue);
			// 创建信号对象
			_createSemaphore();
			//
			// vkDeviceWaitIdle(_vkDevice);
			//
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			if (inst.vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _host->_host, _surface, &surfaceCapabilities) != VK_SUCCESS)
			{
				return false;
			}
			uint32_t formatCount;
			if (inst.vkGetPhysicalDeviceSurfaceFormatsKHR(_host->_host, _surface, &formatCount, nullptr) != VK_SUCCESS)
			{
				return false;
			}
			if (formatCount == 0)
				return false;
			std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
			if (inst.vkGetPhysicalDeviceSurfaceFormatsKHR(_host->_host, _surface, &formatCount, &surfaceFormats[0]) != VK_SUCCESS)
			{
				return false;
			}

			uint32_t nPresentMode;
			if ((inst.vkGetPhysicalDeviceSurfacePresentModesKHR(_host->_host, _surface, &nPresentMode, nullptr) != VK_SUCCESS) ||
				(nPresentMode == 0))
			{
				return false;
			}

			std::vector<VkPresentModeKHR> presentModes(nPresentMode);
			if (inst.vkGetPhysicalDeviceSurfacePresentModesKHR(_host->_host, _surface, &nPresentMode, presentModes.data()) != VK_SUCCESS)
			{
				return false;
			}

			// 1.
			if (surfaceCapabilities.maxImageCount == 0)
			{
				_swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount < 3 ? 3 : surfaceCapabilities.minImageCount;
			}
			else
			{
				_swapchainCreateInfo.minImageCount = surfaceCapabilities.maxImageCount < 3 ? surfaceCapabilities.maxImageCount : 3;
			}
			//desiredImagesCount = max(surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
			// 2.
			VkSurfaceFormatKHR desiredFormat = surfaceFormats[0];
			if ((surfaceFormats.size() == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
			{
				desiredFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
			}
			else
			{
				for (VkSurfaceFormatKHR &surfaceFormat : surfaceFormats)
				{
					if (surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM)
					{
						desiredFormat = surfaceFormat;
						break;
					}
				}
			}

			VkSurfaceTransformFlagBitsKHR desiredTransform;
			VkPresentModeKHR presentMode;

			if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
				desiredTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			else
				desiredTransform = surfaceCapabilities.currentTransform;
			// 6.
			// FIFO present mode is always available
			// MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
			for (VkPresentModeKHR &pmode : presentModes)
			{
				if (pmode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					presentMode = pmode; break;
				}
			}

			if (presentMode != VK_PRESENT_MODE_MAILBOX_KHR)
			{
				for (VkPresentModeKHR &pmode : presentModes)
				{
					if (pmode == VK_PRESENT_MODE_FIFO_KHR)
					{
						presentMode = pmode; break;
					}
				}
			}
			_swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			//_swapchainCI.minImageCount = _swapchainCI.minImageCount;
			_swapchainCreateInfo.imageFormat = desiredFormat.format;
			_swapchainCreateInfo.imageColorSpace = desiredFormat.colorSpace;
			_swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			_swapchainCreateInfo.imageArrayLayers = 1;
			_swapchainCreateInfo.clipped = VK_TRUE;
			_swapchainCreateInfo.surface = _surface;
			_swapchainCreateInfo.flags = 0;
			_swapchainCreateInfo.pNext = nullptr;
			_swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			_swapchainCreateInfo.queueFamilyIndexCount = 0;
			_swapchainCreateInfo.pQueueFamilyIndices = nullptr;
			_swapchainCreateInfo.presentMode = presentMode;
			_swapchainCreateInfo.preTransform = desiredTransform;
			_swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			//
			_createSemaphore();
			_createCommandPool();
			//
			return true;
		}
		//
		View * View::createViewWin32(HWND _hwnd)
		{
			const Instance& inst = GetVkInstance();
			if (!inst)
				return nullptr;
			//
			// Create VkSurfaceKHR
			HINSTANCE hInst = ::GetModuleHandle(0);
			//
			VkWin32SurfaceCreateInfoKHR surface_create_info = {
				VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
				nullptr,                                          // const void                      *pNext
				0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
				hInst,
				_hwnd
			};
			VkSurfaceKHR surface;
			VkResult rst = inst.vkCreateWin32SurfaceKHR(inst, &surface_create_info, nullptr, &surface);
			if (rst == VK_SUCCESS)
			{
				View* view = new View();
				view->_surface = surface;
				return view;
			}
			return nullptr;
		}
	}
}