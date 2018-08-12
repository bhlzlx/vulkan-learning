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
		const Instance& GetVkInstance();

		class View
		{
			friend class Texture2D;
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
			//
			VkCommandPool _commandPool;
			std::vector<VkCommandBuffer> _vecCommandBuffer;
			//
			int _width;
			int _height;
		private:
			View() {}
			~View() {}
			bool _updateSwapchain();
			bool _createSemaphore();
			bool _createCommandPool();
		public:
			bool onResize( int _width, int _height );
			bool begin();
			bool end();
			bool prepareForSwapchain();
			void setHost(Device * _host) { this->_host = _host; }

			VkCommandPool getCommandPool() { return _commandPool;  }
			VkSurfaceKHR getSurface(){ return _surface; }
		public:
			static View* createViewWin32( HWND _hwnd );
		};
	}
}