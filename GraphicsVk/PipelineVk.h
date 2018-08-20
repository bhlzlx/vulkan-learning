#pragma once
#ifdef _WIN32
#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"
#include "clGraphic.h"

namespace clannad
{
	namespace vulkan
	{
		class Device;
		class Pipeline
		{
		private:
			VkViewport _viewport;
			VkRect2D _scissor;
		public:
			Pipeline();
			~Pipeline();
		public:
			static Pipeline* Create(Device* _host, const ClPipelineDesc& _pipelineDesc);
		};

	}
}


