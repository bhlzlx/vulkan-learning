#include "vkDylib.h"
#include <iostream>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
	#undef LoadLibrary
#endif
#include <vknpp/vkTypes.h>
#define __VKNPP_DEFI_FUNCTIONS__
#include "functions/functions.h"

namespace vknpp
{
	bool Dylib::Load()
	{
		m_dylib = ::LoadLibraryA("vulkan-1.dll");
		return !!m_dylib;
	}

	bool Dylib::LoadGlobalApi()
	{
#define VULKAN_EXPORT_API(api)                                                \
    if( !(api = (PFN_##api)GetProcAddress( (HMODULE)m_dylib, #api )) ) {                \
      std::cout << "Could not load vulkan api : " << #api << "!" << std::endl;  \
      return false;                                                                   \
    }
#define VULKAN_GLOBAL_API(api)\
	if (!(api = (PFN_##api)vkGetInstanceProcAddr(nullptr, #api))) {\
	\
      std::cout << "Could not load global level function: " << #api << "!" << std::endl;  \
      return false;                                                                       \
    }
#include "functions/functions.h"
	
		return true;
	}

	bool Dylib::LoadInstanceApi(VkInstance _inst)
	{
#define VULKAN_INSTANCE_API(api)\
	if (!(api = (PFN_##api)vkGetInstanceProcAddr( _inst , #api)))\
	{\
      std::cout << "Could not load global level function: " << #api << "!" << std::endl;\
      return false;\
    }
#include "functions/functions.h"
		return true;
	}

	bool Dylib::LoadDeviceApi( VkDevice _logicDvc)
	{
#define VULKAN_DEVICE_API(api)\
	if (!(api = (PFN_##api)vkGetDeviceProcAddr( _logicDvc, #api))) {\
	\
      std::cout << "Could not load global level function: " << #api << "!" << std::endl;  \
      return false;                                                                       \
    }
#include "functions/functions.h"
		return true;
	}

}