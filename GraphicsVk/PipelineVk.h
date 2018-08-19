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

		class Pipeline
		{
		public:
			Pipeline();
			~Pipeline();
		public:
			static Pipeline* Create(Device* _host, ClVertexLayout _vertLayout);
		};

	}
}


