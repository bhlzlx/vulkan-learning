#ifndef VULKAN_DEVICE_API
#define VULKAN_DEVICE_API( func )
#endif

VULKAN_DEVICE_API(vkGetDeviceQueue)
VULKAN_DEVICE_API(vkDeviceWaitIdle)
VULKAN_DEVICE_API(vkDestroyDevice)

VULKAN_DEVICE_API(vkCreateSemaphore)
VULKAN_DEVICE_API(vkCreateCommandPool)
VULKAN_DEVICE_API(vkAllocateCommandBuffers)
VULKAN_DEVICE_API(vkBeginCommandBuffer)
VULKAN_DEVICE_API(vkCmdPipelineBarrier)
VULKAN_DEVICE_API(vkCmdClearColorImage)
VULKAN_DEVICE_API(vkEndCommandBuffer)
VULKAN_DEVICE_API(vkQueueSubmit)
VULKAN_DEVICE_API(vkQueueWaitIdle)
VULKAN_DEVICE_API(vkFreeCommandBuffers)
VULKAN_DEVICE_API(vkDestroyCommandPool)
VULKAN_DEVICE_API(vkDestroySemaphore)

VULKAN_DEVICE_API(vkCreateSwapchainKHR)
VULKAN_DEVICE_API(vkGetSwapchainImagesKHR)
VULKAN_DEVICE_API(vkAcquireNextImageKHR)
VULKAN_DEVICE_API(vkQueuePresentKHR)
VULKAN_DEVICE_API(vkDestroySwapchainKHR)
// Tutorial 03
VULKAN_DEVICE_API(vkCreateImageView)
VULKAN_DEVICE_API(vkCreateRenderPass)
VULKAN_DEVICE_API(vkCreateFramebuffer)
VULKAN_DEVICE_API(vkCreateShaderModule)
VULKAN_DEVICE_API(vkCreatePipelineLayout)
VULKAN_DEVICE_API(vkCreateGraphicsPipelines)
VULKAN_DEVICE_API(vkCmdBeginRenderPass)
VULKAN_DEVICE_API(vkCmdBindPipeline)
VULKAN_DEVICE_API(vkCmdDraw)
VULKAN_DEVICE_API(vkCmdEndRenderPass)
VULKAN_DEVICE_API(vkDestroyShaderModule)
VULKAN_DEVICE_API(vkDestroyPipelineLayout)
VULKAN_DEVICE_API(vkDestroyPipeline)
VULKAN_DEVICE_API(vkDestroyRenderPass)
VULKAN_DEVICE_API(vkDestroyFramebuffer)
VULKAN_DEVICE_API(vkDestroyImageView)

// Tutorial 04
VULKAN_DEVICE_API(vkCreateFence)
VULKAN_DEVICE_API(vkCreateBuffer)
VULKAN_DEVICE_API(vkGetBufferMemoryRequirements)
VULKAN_DEVICE_API(vkAllocateMemory)
VULKAN_DEVICE_API(vkBindBufferMemory)
VULKAN_DEVICE_API(vkMapMemory)
VULKAN_DEVICE_API(vkFlushMappedMemoryRanges)
VULKAN_DEVICE_API(vkUnmapMemory)
VULKAN_DEVICE_API(vkCmdSetViewport)
VULKAN_DEVICE_API(vkCmdSetScissor)
VULKAN_DEVICE_API(vkCmdBindVertexBuffers)
VULKAN_DEVICE_API(vkWaitForFences)
VULKAN_DEVICE_API(vkResetFences)
VULKAN_DEVICE_API(vkFreeMemory)
VULKAN_DEVICE_API(vkDestroyBuffer)
VULKAN_DEVICE_API(vkDestroyFence)

// buffer copy
VULKAN_DEVICE_API(vkCmdCopyBuffer)
VULKAN_DEVICE_API(vkCmdCopyBufferToImage)
VULKAN_DEVICE_API(vkCmdCopyImageToBuffer)
VULKAN_DEVICE_API(vkCmdCopyImage)
VULKAN_DEVICE_API(vkCmdBlitImage)

// Tutorial 06
VULKAN_DEVICE_API(vkCreateImage)
VULKAN_DEVICE_API(vkGetImageMemoryRequirements)
VULKAN_DEVICE_API(vkBindImageMemory)
VULKAN_DEVICE_API(vkCreateSampler)
VULKAN_DEVICE_API(vkCreateDescriptorSetLayout)
VULKAN_DEVICE_API(vkCreateDescriptorPool)
VULKAN_DEVICE_API(vkAllocateDescriptorSets)
VULKAN_DEVICE_API(vkUpdateDescriptorSets)
VULKAN_DEVICE_API(vkCmdBindDescriptorSets)
VULKAN_DEVICE_API(vkDestroyDescriptorPool)
VULKAN_DEVICE_API(vkDestroyDescriptorSetLayout)
VULKAN_DEVICE_API(vkDestroySampler)
VULKAN_DEVICE_API(vkDestroyImage)

#undef VULKAN_DEVICE_API