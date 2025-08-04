// VulkanModule.h
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>

#include <memory>

#if defined(__APPLE__) && defined(__clang__)
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>

#include <__config>
#endif
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "stb_image.h"
#include "tiny_obj_loader.h"

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>

#include "common.h"


#include "Scene.h"
#include "UniformData.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "vk_mem_alloc.h"

class VulkanModule {
   public:
	VulkanModule(GLFWwindow*, uint32_t width = 800, uint32_t height = 600, int maxFIF = 2);
	void initVulkan();
	void cleanup();
	void drawFrame();
	void recreateSwapChain();
	void waitDeviceIdle();
	void updateUniformBuffer(uint32_t currentImage);

//    private:
	void initWindow();
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	VkImageView createImageView(VkImage image, VkFormat format,
								VkImageAspectFlags aspectFlags, uint32_t mips);
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createColorResources();
	void createDepthResources();
	void cleanupDepthResources();
	void createTextureImage(const std::string &filename);
	void createTextureImageView();
	void createTextureSampler();
	void loadModel(const std::string &filename);
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIdx);
	void createSyncObjects();
	void cleanupSwapChain();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				  VkDebugUtilsMessageTypeFlagsEXT messageType,
				  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
				  void *pUserData);

	void createImage(uint32_t width, uint32_t height, uint32_t mips,
					 VkSampleCountFlagBits numSamples, VkFormat format,
					 VkImageTiling tiling, VkImageUsageFlags usage,
					 VkMemoryPropertyFlags properties, VkImage &image,
					 VkDeviceMemory &imageMemory);

	void transitionImageLayout(VkImage image, VkFormat format,
							   VkImageLayout oldLayout, VkImageLayout newLayout,
							   uint32_t mips);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
						   uint32_t height);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags,
					  VkMemoryPropertyFlags memoryPropertyFlags,
					  VkBuffer &buffer, VkDeviceMemory &deviceMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth,
						 int32_t texHeight, uint32_t mips);

	void populateDebugMessengerCreateInfo(
	  VkDebugUtilsMessengerCreateInfoEXT &createInfo);

	std::vector<const char *> getRequiredExtensions(VkPhysicalDevice device);

	std::vector<const char *> getRequiredExtensions() const;

	int rateDeviceSuitability(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
	  const std::vector<VkSurfaceFormatKHR> &availableFormats);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

	VkPresentModeKHR chooseSwapPresentMode(
	  const std::vector<VkPresentModeKHR> &availablePresentMode);

	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	VkSampleCountFlagBits getMaxUsableSampleCount();

	VkResult CreateDebugUtilsMessengerEXT(
	  VkInstance instance,
	  const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
	  const VkAllocationCallbacks *pAllocator,
	  VkDebugUtilsMessengerEXT *pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
									   VkDebugUtilsMessengerEXT callback,
									   const VkAllocationCallbacks *pAllocator);

	VkFormat findDepthFormat();

	static bool hasStencilComponent(VkFormat format);

	VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
								 VkImageTiling tiling,
								 VkFormatFeatureFlags features);

	static std::vector<char> readFile(const std::string &filename);

	VkShaderModule createShaderModule(const std::vector<char> &code);

	uint32_t findMemoryType(uint32_t typeFilter,
							VkMemoryPropertyFlags properties);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer cmdBuffer);

	static void framebufferResizedCallback(GLFWwindow *window, int width,
										   int height);

	void createSemaphores();  // deprecated

	GLFWwindow *window;

	// Vulkan Core Handles
	VkInstance instance;
	VkDebugUtilsMessengerEXT callback;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	// Swapchain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	// Pipeline
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	// Descriptor
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	// Commands
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	// Depth and Texture
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	// Sync
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;

	bool framebufferResized = false;

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	// wait to be checked
	const std::vector<const char *> validationLayers = {
	  "VK_LAYER_KHRONOS_validation"};

	const std::vector<const char *> deviceExtensions = {
	  VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	std::vector<Vertex> vertices = {};
	std::vector<uint32_t> indices = {};

	std::unique_ptr<Scene> scene;  // include (loaders, lightings ...)

	// Buffers
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void *> uniformBuffersMapped;

	uint32_t WIDTH;
	uint32_t HEIGHT;
	int MAX_FRAMES_IN_FLIGHT;
};
