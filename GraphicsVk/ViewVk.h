#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <VkApiLoader.h>
#include <vector>

namespace clannad
{
	namespace vulkan
	{
		class Device;
		VkInstance GetVkInstance(const InstanceApi*& api_);

		class View
		{
		private:
			Device * _host;
			//
			VkSurfaceKHR _surface = nullptr;
			VkSwapchainKHR _swapchain = nullptr;
			VkQueue _presentQueue;
			VkQueue _graphicQueue;
			VkSwapchainCreateInfoKHR _swapchainCreateInfo;
			std::vector<VkImage> _vecImages;

			VkSemaphore _imageAvail;
			VkSemaphore _cmdExecuted;
			//
			bool _needUpdateSwapchain = true;
			int _width;
			int _height;
		private:
			View();
			~View();
			bool _updateSwapchain();
			bool _createSemaphore();
		public:
			bool onResize( int _width, int _height );
			bool begin();
			bool end();
			bool prepareForSwapchain();
			void setHost(Device * _host) { this->_host = _host; }
			VkSurfaceKHR getSurface(){ return _surface; }
		public:
			static View* createViewWin32( HWND _hwnd );
		};
	}
}