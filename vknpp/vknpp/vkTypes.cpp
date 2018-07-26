#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vkTypes.h"
#include <iostream>
#include <vector>
#include <Windows.h>


namespace vknpp
{
	VkApplicationInfo defAppInfo = 
	{
		VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
		nullptr,                                        // const void                *pNext
		"vknpp api",  // const char                *pApplicationName
		VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   applicationVersion
		"vknpp application",                     // const char                *pEngineName
		VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   engineVersion
		VK_MAKE_VERSION(1, 0, 0)                      // uint32_t                   apiVersion
	};

	const char * defExts[] = 
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
	};

	VkInstanceCreateInfo defInstCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&defAppInfo,
		0,
		nullptr,
		sizeof(defExts) / sizeof(const char *),
		&defExts[0]
	};

	bool Instance::Create(VkInstanceCreateInfo & _ci, Instance & _inst)
	{
		//
		uint32_t nExt = 0;
		if (
			(vkEnumerateInstanceExtensionProperties(nullptr, &nExt, nullptr) != VK_SUCCESS) ||
			(nExt == 0)
			)
		{
			std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
			return false;
		}

		std::vector<VkExtensionProperties> vecExt(nExt);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &nExt, vecExt.data()) != VK_SUCCESS)
		{
			std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
			return false;
		}
		// Create & Return
		return vkCreateInstance(&_ci, nullptr, &_inst.v) == VK_SUCCESS;
	}

	bool Surface::CreateWin32Surface( Instance& _vkInst, void * _hwnd, void * _hInst, Surface& _surface)
	{
		VkWin32SurfaceCreateInfoKHR surface_create_info = {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
			nullptr,                                          // const void                      *pNext
			0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
			(HINSTANCE)_hInst,                                  // HINSTANCE                        hinstance
			(HWND)_hwnd                                  // HWND                             hwnd
		};

		if (vkCreateWin32SurfaceKHR(_vkInst, &surface_create_info, nullptr, &_surface.v) == VK_SUCCESS)
		{
			return true;
		}

		return false;
	}

	bool PhysicalDevice::EnumPhysicalDevices( Instance& _instance, std::vector<PhysicalDevice>& _devices)
	{
		uint32_t nDevice = 0;
		VkResult rst;
		std::vector<VkPhysicalDevice>  phyDevices;
		uint32_t queueFamilyIndex = 0;
		rst = vkEnumeratePhysicalDevices(_instance, &nDevice, nullptr);
		if (rst != VK_SUCCESS || nDevice == 0)
		{
			return false;
		}
		_devices.resize(nDevice);
		rst = vkEnumeratePhysicalDevices(_instance, &nDevice, &_devices.data()->v);
		return rst == VK_SUCCESS;
	}

	VkPhysicalDeviceProperties PhysicalDevice::GetProperties()
	{
		VkPhysicalDeviceProperties prop;
		vkGetPhysicalDeviceProperties(v, &prop);
		return prop;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures()
	{
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(v, &features);
		return features;
	}

	bool PhysicalDevice::SelectQueueFamilies(Surface & _surface)
	{
		this->availGraphicQueueFamily = -1;
		this->availPresentQueueFamily = -1;
		//
		VkPhysicalDeviceProperties device_properties;
		VkPhysicalDeviceFeatures   device_features;
		//
		vkGetPhysicalDeviceProperties(v, &device_properties);
		vkGetPhysicalDeviceFeatures(v, &device_features);

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
		vkGetPhysicalDeviceQueueFamilyProperties(v, &queueFamilyCount, nullptr);
		if (queueFamilyCount == 0)
		{
			return false;
		}
		// select an available queue family
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(v, &queueFamilyCount, queueFamilyProperties.data());
		uint32_t index = 0;
		for (const VkQueueFamilyProperties& property : queueFamilyProperties)
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(v, index, _surface, &presentSupport);
			if (property.queueCount > 0 && property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				availGraphicQueueFamily = index;
			}
			if (presentSupport)
			{
				availPresentQueueFamily = index;
				return true;
			}
			++index;
		}
		return false;
	}

	bool LogicalDevice::Create(PhysicalDevice & _physicalDevice, LogicalDevice & _logicalDevice)
	{
		std::vector<float> queuePriorities = { 1.0f };
		VkDeviceQueueCreateInfo queueCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void                  *pNext
			0,                                              // VkDeviceQueueCreateFlags     flags
			_physicalDevice.availGraphicQueueFamily,                    // uint32_t                     queueFamilyIndex
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
		_logicalDevice.physicalDevice = _physicalDevice;
		auto rst = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_logicalDevice.v);
		return rst == VK_SUCCESS;
	}

	bool LogicalDevice::SelectQueues()
	{
		vkGetDeviceQueue( v, physicalDevice.availGraphicQueueFamily, 0, &graphicQueue);
		vkGetDeviceQueue(v, physicalDevice.availPresentQueueFamily, 0, &presentQueue);
		return true;
	}

	bool LogicalDevice::CreateSwapchain(Surface & _surface)
	{
		return this->swapchain.Initialize(*this, _surface);		
	}

	bool LogicalDevice::CreateSemaphore(VkSemaphore & _semaphore)
	{
		VkSemaphoreCreateInfo semaphore_create_info = {
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,      // VkStructureType          sType
			nullptr,                                      // const void*              pNext
			0                                             // VkSemaphoreCreateFlags   flags
		};
		return vkCreateSemaphore(v, &semaphore_create_info, nullptr, &_semaphore) == VK_SUCCESS;
	}

	bool Swapchain::Initialize(LogicalDevice & _device, Surface & _surface)
	{
		if (!_device)
			return false;
		//
		vkDeviceWaitIdle(_device);
		//
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _device.physicalDevice, _surface, &surfaceCapabilities) != VK_SUCCESS)
		{
			std::cout << "Could not check presentation surface capabilities!" << std::endl;
			return false;
		}
		uint32_t formatCount;
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(_device.physicalDevice, _surface, &formatCount, nullptr) != VK_SUCCESS)
			return false;
		if (formatCount == 0)
			return false;

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);

		if (vkGetPhysicalDeviceSurfaceFormatsKHR(_device.physicalDevice, _surface, &formatCount, &surfaceFormats[0]) != VK_SUCCESS)
		{
			std::cout << "Error occurred during presentation surface formats enumeration!" << std::endl;
			return false;
		}

		uint32_t nPresentMode;
		if ((vkGetPhysicalDeviceSurfacePresentModesKHR( _device.physicalDevice, _surface, &nPresentMode, nullptr) != VK_SUCCESS) ||
			(nPresentMode == 0))
		{
			std::cout << "Error occurred during presentation surface present modes enumeration!" << std::endl;
			return false;
		}

		std::vector<VkPresentModeKHR> presentModes(nPresentMode);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(_device.physicalDevice, _surface, &nPresentMode, presentModes.data()) != VK_SUCCESS)
		{
			std::cout << "Error occurred during presentation surface present modes enumeration!" << std::endl;
			return false;
		}

		// 1.
		if (surfaceCapabilities.maxImageCount == 0)
		{
			desiredImagesCount = surfaceCapabilities.minImageCount + 1;
		}
		else
		{
			desiredImagesCount = surfaceCapabilities.maxImageCount;
		}
		//desiredImagesCount = max(surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
		// 2.
		desiredFormat = surfaceFormats[0];
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
		// 3.
		if (surfaceCapabilities.currentExtent.width == -1)
		{
			desiredExtend = { surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height };
		}
		else
		{
			desiredExtend = surfaceCapabilities.currentExtent;
		}
		// 4.
		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		{
			desiredUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		else
		{
			desiredUsage = static_cast<VkImageUsageFlags>(-1);
		}
		// 5.
		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			desiredTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		else
			desiredTransform = surfaceCapabilities.currentTransform;
		// 6.
		// FIFO present mode is always available
		// MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
		for (VkPresentModeKHR &pmode : presentModes)
			if (pmode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				desiredPresentMode = pmode; goto  GET_PRESENT_MODE_FINISHED;
			}

		for (VkPresentModeKHR &pmode : presentModes)
			if (pmode == VK_PRESENT_MODE_FIFO_KHR)
			{
				desiredPresentMode = pmode; goto  GET_PRESENT_MODE_FINISHED;
			}
		return false;
	GET_PRESENT_MODE_FINISHED:

		if (static_cast<int>(desiredUsage) == -1)
			return false;
		if (static_cast<int>(desiredPresentMode) == -1)
			return false;

		if ((desiredExtend.width == 0) || (desiredExtend.height == 0))
		{
			// Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
			// But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
			return true;
		}

		oldSwapchain = v;

		VkSwapchainCreateInfoKHR swap_chain_create_info = {
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                sType
			nullptr,                                      // const void                    *pNext
			0,                                            // VkSwapchainCreateFlagsKHR      flags
			_surface,                   // VkSurfaceKHR                   surface
			desiredImagesCount,                     // uint32_t                       minImageCount
			desiredFormat.format,                        // VkFormat                       imageFormat
			desiredFormat.colorSpace,                    // VkColorSpaceKHR                imageColorSpace
			desiredExtend,                               // VkExtent2D                     imageExtent
			1,                                            // uint32_t                       imageArrayLayers
			desiredUsage,                                // VkImageUsageFlags              imageUsage
			VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                  imageSharingMode
			0,                                            // uint32_t                       queueFamilyIndexCount
			nullptr,                                      // const uint32_t                *pQueueFamilyIndices
			desiredTransform,                            // VkSurfaceTransformFlagBitsKHR  preTransform
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR    compositeAlpha
			desiredPresentMode,                         // VkPresentModeKHR               presentMode
			VK_TRUE,                                      // VkBool32                       clipped
			oldSwapchain                                // VkSwapchainKHR                 oldSwapchain
		};

		if (vkCreateSwapchainKHR( _device, &swap_chain_create_info, nullptr, &this->Get()) != VK_SUCCESS)
		{
			std::cout << "Could not create swap chain!" << std::endl;
			return false;
		}
		if (oldSwapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(_device, oldSwapchain, nullptr);
		}
		return true;
	}

	bool Swapchain::CreateCommandBuffers(LogicalDevice & _device)
	{
		VkCommandPoolCreateInfo cmdPoolCI = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			0,                                              // VkCommandPoolCreateFlags     flags
			_device.physicalDevice.availGraphicQueueFamily,                 // uint32_t                     queueFamilyIndex
		};

		if (vkCreateCommandPool( _device, &cmdPoolCI, nullptr, &commandPool) != VK_SUCCESS)
		{
			std::cout << "Could not create a command pool!" << std::endl;
			return false;
		}

		this->commandBuffers.resize(this->swapchainImages.size());

		VkCommandBufferAllocateInfo cmd_buffer_allocate_info =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType              sType
			nullptr,                                        // const void*                  pNext
			commandPool,                     // VkCommandPool                commandPool
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel         level
			static_cast<uint32_t>(swapchainImages.size())                                     // uint32_t                     bufferCount
		};
		if (vkAllocateCommandBuffers(_device, &cmd_buffer_allocate_info, &this->commandBuffers[0]) != VK_SUCCESS)
		{
			std::cout << "Could not allocate command buffers!" << std::endl;
			return false;
		}
		return true;
	}

	bool Swapchain::CreateImages(LogicalDevice& _device)
	{
		uint32_t swapchainCount = 0;
		vkGetSwapchainImagesKHR(_device, v, &swapchainCount, nullptr);
		this->swapchainImages.assign(swapchainCount, VkImage());
		vkGetSwapchainImagesKHR(_device, v, &swapchainCount, &swapchainImages[0]);
		return !!swapchainCount;
	}

	bool Swapchain::RecordCommandBuffers(LogicalDevice & _device)
	{
		auto buffer_count = this->commandBuffers.size();

		VkCommandBufferBeginInfo cmd_buffer_begin_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,  // VkStructureType                        sType
			nullptr,                                      // const void                            *pNext
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, // VkCommandBufferUsageFlags              flags
			nullptr                                       // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
		};

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

		for (uint32_t i = 0; i < buffer_count; ++i)
		{
			VkImageMemoryBarrier barrier_from_present_to_clear = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               srcQueueFamilyIndex
				VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               dstQueueFamilyIndex
				swapchainImages[i],                       // VkImage                                image
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
				swapchainImages[i],                       // VkImage                                image
				image_subresource_range                     // VkImageSubresourceRange                subresourceRange
			};

			vkBeginCommandBuffer( this->commandBuffers[i], &cmd_buffer_begin_info);
			vkCmdPipelineBarrier(this->commandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);

			vkCmdClearColorImage(this->commandBuffers[i], swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range);

			vkCmdPipelineBarrier(this->commandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);

			if (vkEndCommandBuffer(this->commandBuffers[i]) != VK_SUCCESS) 
			{
				std::cout << "Could not record command buffers!" << std::endl;
				return false;
			}
		}
		return true;
	}
}

