// VkApiLoaderConsole.cpp : �������̨Ӧ�ó������ڵ㡣
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

