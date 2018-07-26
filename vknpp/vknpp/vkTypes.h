#pragma once
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <vector>

#define __VKNPP_DECL_FUNCTIONS__
#include "functions/functions.h"

namespace vknpp
{
	extern VkApplicationInfo defAppInfo;
	extern const char * defExts[];
	extern VkInstanceCreateInfo defInstCreateInfo;
	//
#define VK_TYPE_WRAP( TYPE ) operator TYPE() { return v; }\
TYPE & Get(){ return v;}
	struct Instance
	{
		VkInstance v;
		VK_TYPE_WRAP(VkInstance)
		static bool Create( VkInstanceCreateInfo& _ci, Instance & _inst );
	};

	struct Surface
	{
		VkSurfaceKHR v;
		VK_TYPE_WRAP(VkSurfaceKHR)
		static bool CreateWin32Surface( Instance& _vkInst, void * _hwnd, void * _hInst, Surface& _surface);
	};

	struct PhysicalDevice
	{
		VkPhysicalDevice v;
		//
		uint32_t availGraphicQueueFamily;
		uint32_t availPresentQueueFamily;

		VK_TYPE_WRAP(VkPhysicalDevice)
		//
		static bool EnumPhysicalDevices( Instance& _instance, std::vector<PhysicalDevice>& _devices );
		VkPhysicalDeviceProperties GetProperties();
		VkPhysicalDeviceFeatures GetFeatures();
		bool SelectQueueFamilies(Surface& _surface);
	};
#undef CreateSemaphore

	struct LogicalDevice;
	struct Swapchain
	{
		VkSwapchainKHR v;

		VK_TYPE_WRAP(VkSwapchainKHR)
		//
		uint32_t desiredImagesCount = 0;
		VkSurfaceFormatKHR desiredFormat;
		VkExtent2D desiredExtend;
		VkImageUsageFlags desiredUsage;
		VkSurfaceTransformFlagBitsKHR desiredTransform;
		VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_MAX_ENUM;
		VkSwapchainKHR oldSwapchain = 0;

		//
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageviews;
		//
		bool Initialize(LogicalDevice&_physicalDevice, Surface& _surface);
		bool CreateCommandBuffers(LogicalDevice&_physicalDevice);
		bool CreateImages( LogicalDevice& _device);
		bool RecordCommandBuffers(LogicalDevice& _device);
	};

	struct LogicalDevice
	{
		VkDevice v;
		VK_TYPE_WRAP(VkDevice)
		//
		PhysicalDevice physicalDevice;
		//
		Swapchain swapchain;
		//
		VkQueue graphicQueue;
		VkQueue presentQueue;
		//
		static bool Create(PhysicalDevice & _physicalDevice, LogicalDevice & _logicalDevice);
		bool SelectQueues();
		bool CreateSwapchain( Surface& _surface );
		bool CreateSemaphore(VkSemaphore&);
	};
};