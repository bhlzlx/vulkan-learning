// VkApiLoaderConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VkApiLoader.h"

int main()
{
	clannad::vulkan::ApiLoader apiLoader;
	bool rst = apiLoader.Initialize();
	VkInstance instance;
	VkDevice device;
	clannad::vulkan::DeviceLoadData deviceLoadData;
	instance = apiLoader.CreateInstance();
	rst = apiLoader.CreateDevice(instance,0,deviceLoadData);
    return 0;
}

