#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "Device.h"
#include <vulkan/vulkan.h>
#include "vknpp/vkDylib.h"
#include "vknpp/vkTypes.h"
#include "vkCustomDesc.h"

#include <windows.h>

namespace iflib
{

	Device::Device()
	{
		_dynlib = nullptr;
	}


	Device::~Device()
	{
	}

	bool Device::_createVulkanInstance()
	{
		uint32_t nExt = 0;
		VkResult rst = vkEnumerateInstanceExtensionProperties(nullptr, &nExt, nullptr);
		if (rst != VK_SUCCESS) 
			return false;
		if (0 == nExt)
			return false;
		//
		std::vector<VkExtensionProperties> vecExt(nExt);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &nExt, vecExt.data()) != VK_SUCCESS)
		{
			return false;
		}
		// Create & Return
		return vkCreateInstance(&IflibVkInstanceCreateInfo, nullptr, &_vkInst) == VK_SUCCESS;
	}

	bool Device::_createVulkanSurface(void* _hWnd, VkSurfaceKHR& _surface)
	{
		HINSTANCE hInst = ::GetModuleHandle(0);
		HWND hWnd = (HWND)_hWnd;
		//
		VkWin32SurfaceCreateInfoKHR surface_create_info = {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
			nullptr,                                          // const void                      *pNext
			0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
			hInst,
			hWnd
		};
		VkResult rst = vkCreateWin32SurfaceKHR(_vkInst, &surface_create_info, nullptr, &_surface);
		return rst == VK_SUCCESS;
	}

	uint32_t Device::_enumPhysicalDevices( VkPhysicalDevice (&_arrDevice)[8] )
	{
		uint32_t nDevice = 0;
		VkResult rst;
		uint32_t queueFamilyIndex = 0;
		rst = vkEnumeratePhysicalDevices( _vkInst, &nDevice, nullptr);
		if (rst != VK_SUCCESS || nDevice == 0)
		{
			return 0;
		}
		if (nDevice > 8)
		{
			nDevice = 8;
		}
		rst = vkEnumeratePhysicalDevices(_vkInst, &nDevice, &_arrDevice[0]);
		if (rst != VK_SUCCESS)
			return 0;
		return nDevice;
	}

	bool Device::_selectPhysicalDevice( VkPhysicalDevice _phy, VkSurfaceKHR& _surface)
	{
		uint32_t graphicQueueFamily = -1;
		uint32_t presentQueueFamily = -1;
		//
		VkPhysicalDeviceProperties device_properties;
		VkPhysicalDeviceFeatures   device_features;
		//
		vkGetPhysicalDeviceProperties(_phy, &device_properties);
		vkGetPhysicalDeviceFeatures(_phy, &device_features);

		uint32_t major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
		uint32_t minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
		uint32_t patch_version = VK_VERSION_PATCH(device_properties.apiVersion);
		// simple function support test
		if ((major_version < 1) || (device_properties.limits.maxImageDimension2D < 4096))
		{
			return false;
		}
		// retrieve queue family count
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_phy, &queueFamilyCount, nullptr);
		if (queueFamilyCount == 0)
		{
			return false;
		}
		// select an available queue family
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_phy, &queueFamilyCount, queueFamilyProperties.data());
		uint32_t index = 0;
		for (const VkQueueFamilyProperties& property : queueFamilyProperties)
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_phy, index, _surface, &presentSupport);
			if (property.queueCount > 0 && property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicQueueFamily = index;
			}
			if (presentSupport)
			{
				presentQueueFamily = index;
				return true;
			}
			++index;
		}
		return false;
	}

	bool Device::_initializeLogicalDevice(VkPhysicalDevice _phy, VkDevice& _device)
	{
		std::vector<float> queuePriorities = { 1.0f };
		VkDeviceQueueCreateInfo queueCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void                  *pNext
			0,                                              // VkDeviceQueueCreateFlags     flags
			_vkGraphicQueueFamily,                    // uint32_t                     queueFamilyIndex
			static_cast<uint32_t>(queuePriorities.size()), // uint32_t                     queueCount
			queuePriorities.data()                         // const float                 *pQueuePriorities
		};
		VkDeviceCreateInfo deviceCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           // VkStructureType                    sType
			nullptr,                                        // const void                        *pNext
			0,                                              // VkDeviceCreateFlags                flags
			1,                                              // uint32_t                           queueCreateInfoCount
			&queueCreateInfo,                             // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
			0,                                              // uint32_t                           enabledLayerCount
			nullptr,                                        // const char * const                *ppEnabledLayerNames
			0,                                              // uint32_t                           enabledExtensionCount
			nullptr,                                        // const char * const                *ppEnabledExtensionNames
			nullptr                                         // const VkPhysicalDeviceFeatures    *pEnabledFeatures
		};
		auto rst = vkCreateDevice( _vkPhyDevice, &deviceCreateInfo, nullptr, &_device);
		return rst == VK_SUCCESS;
	}

	View* Device::createView(void * _hWnd)
	{
		View* pView = new View( this );
		bool rst = false;
		_createVulkanSurface(_hWnd, pView->_vkSurface);
		VkPhysicalDevice arrPhyDvc[8] = { nullptr };
		uint32_t nPhy = _enumPhysicalDevices(arrPhyDvc);
		if (nPhy)
		{
			rst = _selectPhysicalDevice(arrPhyDvc[nPhy - 1], pView->_vkSurface);
			if (rst)
			{
				rst = _initializeLogicalDevice(arrPhyDvc[nPhy - 1], pView->_vkDevice);
				if (rst)
				{
					pView->_presentQueueFamily = _vkPresentQueueFamily;
					pView->_graphicQueueFamily = _vkGraphicQueueFamily;
					return pView;
				}
			}
		}
		delete pView;
		return nullptr;
	}

	bool Device::Initialize(void * _hWnd)
	{
		bool rst = false;
		// load dynamic library ( vulkan-1.dll )
		this->_dynlib = new vknpp::Dylib();
		vknpp::Dylib& dylib = *this->_dynlib;
		// 1. dll
		rst = dylib.Load();
		if (!rst)
			return false;
		// 2. exported api
		rst = dylib.LoadGlobalApi();
		if (!rst)
			return false;
		this->_createVulkanInstance();
		// 3. instance api
		rst = dylib.LoadInstanceApi(this->_vkInst);
		if (!rst)
		{
			return false;
		}
		// 4. create Surface, select physical device && create logical device
		this->_mainView = new View(this);
		this->_createVulkanSurface(_hWnd, this->_mainView->_vkSurface);
		VkPhysicalDevice arrPhyDvc[8] = { nullptr };
		uint32_t nPhy = this->_enumPhysicalDevices(arrPhyDvc);
		if (!nPhy)
			return false;
		rst = this->_selectPhysicalDevice(arrPhyDvc[nPhy - 1], this->_mainView->_vkSurface);
		if (!rst)
			return false;
		rst = this->_initializeLogicalDevice(arrPhyDvc[nPhy - 1], this->_mainView->_vkDevice);
		if (!rst)
			return false;
		// 5. device api
		rst = dylib.LoadDeviceApi(this->_mainView->_vkDevice);
		if (!rst)
		{
			return false;
		}
		return true;
	}

	View::View(Device * _host)
	{
		this->_host = _host;
		_vkSurface = nullptr;
		_vkDevice = nullptr;
		_presentQueueFamily = -1;
		_graphicQueueFamily = -1;
		_vkPresentQueue = nullptr;
		_vkGraphicQueue = nullptr;
		_vkSwapchain = nullptr;
		_needUpdateSwapchain = true;
	}

	bool View::initialize()
	{
		if (!_host || !_vkDevice)
			return false;
		// get command queues
		// 实际上在创建逻辑设备的时候这些队列都已经生成好，这里仅仅里取出来句柄
		vkGetDeviceQueue(_vkDevice, _graphicQueueFamily, 0, &_vkGraphicQueue);
		vkGetDeviceQueue(_vkDevice, _presentQueueFamily, 0, &_vkPresentQueue);
		// 创建信号对象
		_createSemaphore();
		//
		// vkDeviceWaitIdle(_vkDevice);
		//
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _host->get(), _vkSurface, &surfaceCapabilities) != VK_SUCCESS)
		{
			return false;
		}
		uint32_t formatCount;
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(_host->get(), _vkSurface, &formatCount, nullptr) != VK_SUCCESS)
			return false;
		if (formatCount == 0)
			return false;
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(_host->get(), _vkSurface, &formatCount, &surfaceFormats[0]) != VK_SUCCESS)
		{
			return false;
		}

		uint32_t nPresentMode;
		if ((vkGetPhysicalDeviceSurfacePresentModesKHR(_host->get(), _vkSurface, &nPresentMode, nullptr) != VK_SUCCESS) ||
			(nPresentMode == 0))
		{
			return false;
		}

		std::vector<VkPresentModeKHR> presentModes(nPresentMode);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(_host->get(), _vkSurface, &nPresentMode, presentModes.data()) != VK_SUCCESS)
		{
			return false;
		}

		// 1.
		if (surfaceCapabilities.maxImageCount == 0)
		{
			_swapchainCI.minImageCount = surfaceCapabilities.minImageCount < 3 ? 3 : surfaceCapabilities.minImageCount;
		}
		else
		{
			_swapchainCI.minImageCount = surfaceCapabilities.maxImageCount < 3 ? surfaceCapabilities.maxImageCount : 3;
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
		
		_swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		//_swapchainCI.minImageCount = _swapchainCI.minImageCount;
		_swapchainCI.imageFormat = desiredFormat.format;
		_swapchainCI.imageColorSpace = desiredFormat.colorSpace;
		_swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		_swapchainCI.imageArrayLayers = 1;
		_swapchainCI.clipped = VK_TRUE;
		_swapchainCI.surface = _vkSurface;
		_swapchainCI.flags = 0;
		_swapchainCI.pNext = nullptr;
		_swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		_swapchainCI.queueFamilyIndexCount = 0;
		_swapchainCI.pQueueFamilyIndices = nullptr;
		_swapchainCI.presentMode = presentMode;
		_swapchainCI.preTransform = desiredTransform;
		_swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			
		return true;
	}

	void View::begin()
	{
		if (_needUpdateSwapchain)
		{
			if (!_updateSwapchain())
			{
				return;
			}
		}

		uint32_t availImageIndex;
		VkResult result = vkAcquireNextImageKHR( _vkDevice, _vkSwapchain, UINT64_MAX, _vkImageAvailSem, VK_NULL_HANDLE, &availImageIndex);
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
			&_vkImageAvailSem,              // const VkSemaphore           *pWaitSemaphores
			&wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
			1,                                            // uint32_t                     commandBufferCount
			&logicalDevice.swapchain.commandBuffers[image_index],  // const VkCommandBuffer       *pCommandBuffers
			1,                                            // uint32_t                     signalSemaphoreCount
			&drawFinished            // const VkSemaphore           *pSignalSemaphores
		};

		if (vkQueueSubmit(logicalDevice.presentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
			return false;
		}

		VkPresentInfoKHR present_info = {
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&drawFinished,           // const VkSemaphore           *pWaitSemaphores
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
			return Resize(0, 0);
		default:
			std::cout << "Problem occurred during image presentation!" << std::endl;
			return false;
		}
		return true;
		//
		//
	}

	void View::end()
	{
	}

	void View::onSizeChanged(int _width, int _height)
	{
		if (_width != this->_width || _height != this->_height)
		{
			this->_width = _width;
			this->_height = _height;
			_needUpdateSwapchain = true;
		}
	}

	bool View::_updateSwapchain()
	{
		//
		vkDeviceWaitIdle(_vkDevice);
		//
		_swapchainCI.oldSwapchain = _vkSwapchain;
		if (0 == _swapchainCI.imageExtent.width || 0 == _swapchainCI.imageExtent.height)
		{
			return false;
		}
		VkSwapchainKHR oldSwapchain = _vkSwapchain;
		if (vkCreateSwapchainKHR( _vkDevice, &_swapchainCI, nullptr, &_vkSwapchain) != VK_SUCCESS)
		{
			return false;
		}
		if (oldSwapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(_vkDevice, oldSwapchain, nullptr);
		}
		// 获取swapchain上的纹理
		uint32_t nImage = 0;
		vkGetSwapchainImagesKHR( _vkDevice, _vkSwapchain, &nImage, nullptr);
		if (!nImage)
			return false;
		_vecImages.resize(nImage);
		vkGetSwapchainImagesKHR(_vkDevice, _vkSwapchain, &nImage, &_vecImages[0]);
		_needUpdateSwapchain = false;
		return true;
	}

	bool View::_createSemaphore()
	{
		VkSemaphoreCreateInfo semaphore_create_info = {
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,      // VkStructureType          sType
			nullptr,                                      // const void*              pNext
			0                                             // VkSemaphoreCreateFlags   flags
		};
		if (!vkCreateSemaphore(_vkDevice, &semaphore_create_info, nullptr, &_vkImageAvailSem) == VK_SUCCESS)
			return false;
		if (!vkCreateSemaphore(_vkDevice, &semaphore_create_info, nullptr, &_vkDrawFinishedSem) == VK_SUCCESS)
			return false;
		return true;
	}

};