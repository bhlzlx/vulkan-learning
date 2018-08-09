#ifdef __VKNPP_DECL_FUNCTIONS__
#define VULKAN_EXPORT_API( func ) extern PFN_##func func;
#define VULKAN_GLOBAL_API( func ) extern PFN_##func func;
#define VULKAN_INSTANCE_API( func ) extern PFN_##func func;
#define VULKAN_DEVICE_API( func ) extern PFN_##func func;
extern "C"
{
#endif

#ifdef __VKNPP_DEFI_FUNCTIONS__
#define VULKAN_EXPORT_API( func ) PFN_##func func;
#define VULKAN_GLOBAL_API( func ) PFN_##func func;
#define VULKAN_INSTANCE_API( func ) PFN_##func func;
#define VULKAN_DEVICE_API( func ) PFN_##func func;
	extern "C"
	{
#endif

#include "exportFunctions.inl"
#include "globalFunctions.inl"
#include "instanceFunctions.inl"
#include "deviceFunctions.inl"

#ifdef __VKNPP_DECL_FUNCTIONS__
}
#endif

#ifdef __VKNPP_DEFI_FUNCTIONS__
}
#endif

#undef __VKNPP_DECL_FUNCTIONS__
#undef __VKNPP_DEFI_FUNCTIONS__