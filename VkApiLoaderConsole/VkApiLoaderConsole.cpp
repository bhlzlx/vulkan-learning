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
	clannad::vulkan::InstanceApi instApi;
	clannad::vulkan::DeviceApi deviceApi;
	clannad::vulkan::DeviceLoadData deviceLoadData;
	rst = apiLoader.CreateInstance(instance, instApi);
	rst = apiLoader.CreateDevice(instApi, instance,0,deviceLoadData, deviceApi);
    return 0;
}

