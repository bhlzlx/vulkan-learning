#pragma once
#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include <vector>

namespace vknpp
{
	class Dylib;
}

namespace iflib
{
	class View;

	class Device
	{
	private:
		vknpp::Dylib* _dynlib;
	private:
		View* _mainView;
	public:
		Device();
		~Device();
	private:
		VkInstance _vkInst;
		VkPhysicalDevice _vkPhyDevice;
		uint32_t _vkGraphicQueueFamily;
		uint32_t _vkPresentQueueFamily;
		//
	private:
		bool _createVulkanInstance();
		bool _createVulkanSurface( void* _hWnd, VkSurfaceKHR& _surface );
		uint32_t _enumPhysicalDevices(VkPhysicalDevice(&_arrDevice)[8]);
		bool _selectPhysicalDevice(VkPhysicalDevice _phy, VkSurfaceKHR& _surface );
		bool _initializeLogicalDevice(VkPhysicalDevice _phy, VkDevice& _device);
	public:
		View* createView(void* _hWnd);
	public:
		bool Initialize(void* _hWnd);
		VkPhysicalDevice get()
		{
			return _vkPhyDevice;
		}
	};

	class View
	{
	public:
		friend class Device;
		struct Description
		{

		};
	private:
		Device* _host;
		VkSurfaceKHR _vkSurface;
		VkDevice _vkDevice;
		uint32_t _presentQueueFamily;
		uint32_t _graphicQueueFamily;
		VkQueue _vkPresentQueue;
		VkQueue _vkGraphicQueue;
		//
		VkSemaphore _vkImageAvailSem;
		VkSemaphore _vkDrawFinishedSem;
		//
		VkSwapchainCreateInfoKHR _swapchainCI;
		VkSwapchainKHR _vkSwapchain;
		std::vector<VkImage> _vecImages;
		bool _needUpdateSwapchain;
	private:
		int _width;
		int _height;
	private:
		View(Device* _host);
	public:
		void onSizeChanged(int _width, int _height);
		bool initialize();
		void begin();

		void end();
	private:
		bool _updateSwapchain();
		bool _createSemaphore();
	};
}
