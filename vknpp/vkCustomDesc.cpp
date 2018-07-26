#include "vkCustomDesc.h"

static VkApplicationInfo IflibVkApplicationInfo =
{
	VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
	nullptr,                                        // const void                *pNext
	"iflib api",  // const char                *pApplicationName
	VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   applicationVersion
	"iflib graphics api",                     // const char                *pEngineName
	VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   engineVersion
	VK_MAKE_VERSION(1, 0, 0)                      // uint32_t                   apiVersion
};

const char * IflibVkExtentionPlist[] =
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

VkInstanceCreateInfo IflibVkInstanceCreateInfo = {
	VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	nullptr,
	0,
	&IflibVkApplicationInfo,
	0,
	nullptr,
	sizeof(IflibVkExtentionPlist) / sizeof(const char *),
	&IflibVkExtentionPlist[0]
};