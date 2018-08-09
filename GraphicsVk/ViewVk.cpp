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
		VkInstance GetVkInstance(const InstanceApi*& api_)
		{
			static VkInstance vkInst = nullptr;
			static InstanceApi InstApi;
			if (!vkInst)
			{
				auto apiLoader = GetApiLoader();
				bool rst = apiLoader->CreateInstance(vkInst, InstApi);
				if (!rst)
					return nullptr;
			}
			api_ = &InstApi;
			return vkInst;
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
		bool View::_updateSwapchain()
		{
			const InstanceApi* instApi = nullptr;
			VkInstance inst = GetVkInstance(instApi);
			if (!inst)
				return nullptr;
			//
			VkDevice &device = _host->_id;
			DeviceApi &api = _host->_api;
			api.vkDeviceWaitIdle( device );
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
		}

		bool View::begin()
		{
			if (_needUpdateSwapchain)
			{
				_updateSwapchain();
			}
			//
			uint32_t availImageIndex;
			VkResult result = vkAcquireNextImageKHR( _host->_id, _swapchain, UINT64_MAX, _imageAvail, VK_NULL_HANDLE, &availImageIndex);
			switch (result)
			{
			case VK_SUCCESS:
			case VK_SUBOPTIMAL_KHR:
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				_needUpdateSwapchain = true;
				return;
			default:
				return;
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
				&logicalDevice.swapchain.commandBuffers[image_index],  // const VkCommandBuffer       *pCommandBuffers
				1,                                            // uint32_t                     signalSemaphoreCount
				&_cmdExecuted            // const VkSemaphore           *pSignalSemaphores
			};

			if (vkQueueSubmit(logicalDevice.presentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
				return false;
			}

			VkPresentInfoKHR present_info = {
				VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
				nullptr,                                      // const void                  *pNext
				1,                                            // uint32_t                     waitSemaphoreCount
				&_cmdExecuted,           // const VkSemaphore           *pWaitSemaphores
				1,                                            // uint32_t                     swapchainCount
				(VkSwapchainKHR*)&logicalDevice.swapchain,                            // const VkSwapchainKHR        *pSwapchains
				&image_index,                                 // const uint32_t              *pImageIndices
				nullptr                                       // VkResult                    *pResults
			};
			result = vkQueuePresentKHR(logicalDevice.presentQueue, &present_info);

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
		}

		//
		bool View::prepareForSwapchain()
		{
			if (!_host)
				return false;
			// get command queues
			VkDevice &device = _host->_id;
			DeviceApi &api = _host->_api;
			const InstanceApi* instApi;
			VkInstance inst = GetVkInstance( instApi );
			// 实际上在创建逻辑设备的时候这些队列都已经生成好，这里仅仅里取出来句柄
			api.vkGetDeviceQueue(device, _host->_graphicQueueFamily, 0, &_graphicQueue);
			api.vkGetDeviceQueue(device, _host->_presentQueueFamily, 0, &_presentQueue);
			// 创建信号对象
			_createSemaphore();
			//
			// vkDeviceWaitIdle(_vkDevice);
			//
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			if (instApi->vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _host->_host, _surface, &surfaceCapabilities) != VK_SUCCESS)
			{
				return false;
			}
			uint32_t formatCount;
			if (instApi->vkGetPhysicalDeviceSurfaceFormatsKHR(_host->_host, _surface, &formatCount, nullptr) != VK_SUCCESS)
			{
				return false;
			}
			if (formatCount == 0)
				return false;
			std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
			if (instApi->vkGetPhysicalDeviceSurfaceFormatsKHR(_host->_host, _surface, &formatCount, &surfaceFormats[0]) != VK_SUCCESS)
			{
				return false;
			}

			uint32_t nPresentMode;
			if ((instApi->vkGetPhysicalDeviceSurfacePresentModesKHR(_host->_host, _surface, &nPresentMode, nullptr) != VK_SUCCESS) ||
				(nPresentMode == 0))
			{
				return false;
			}

			std::vector<VkPresentModeKHR> presentModes(nPresentMode);
			if (instApi->vkGetPhysicalDeviceSurfacePresentModesKHR(_host->_host, _surface, &nPresentMode, presentModes.data()) != VK_SUCCESS)
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

			return true;
		}
		//
		View * View::createViewWin32(HWND _hwnd)
		{
			const InstanceApi* instApi = nullptr;
			VkInstance inst = GetVkInstance(instApi);
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
			VkResult rst = instApi->vkCreateWin32SurfaceKHR(inst, &surface_create_info, nullptr, &surface);
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