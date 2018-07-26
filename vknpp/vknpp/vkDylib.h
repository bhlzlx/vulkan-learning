#pragma once
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#define __VKNPP_DECL_FUNCTIONS__
#include "functions/functions.h"

namespace vknpp
{
	struct Instance;
	struct LogicalDevice;
	//
	struct Dylib
	{
	private:
		void * m_dylib;
	public:
		bool Load();
		bool LoadGlobalApi();
		bool LoadInstanceApi( VkInstance _inst );
		bool LoadDeviceApi( VkDevice _logicDvc );
	};
}