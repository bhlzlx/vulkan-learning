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
	clannad::vulkan::Instance inst;
	clannad::vulkan::DeviceLoadData deviceLoadData;
	rst = apiLoader.CreateInstance(inst);
	rst = apiLoader.CreateDevice(inst,0, deviceLoadData);
    return 0;
}

