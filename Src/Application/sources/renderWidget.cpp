#include "RenderWidget.h"

RenderWidget::RenderWidget(GLFWwindow *window, uint32_t x, uint32_t y, uint32_t width,
						   uint32_t height, bool vis, bool resize,
						   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
#if defined(__USE_VULKAN__)
	printf("loading Vulkan module......\n");
	loadVulkan();
#elif defined(__USE_OPENGL__)
#elif defined(__USE_DX11__)
#elif defined(__USE_DX12__)
#endif
}

RenderWidget::~RenderWidget() {}

void RenderWidget::display() {
	if(vulkanModule) {
		try {
			vulkanModule->drawFrame();
		} catch (const std::exception& e) {
			std::printf("Vulkan draw frame error: %s\n", e.what());
		}
	}
}

void RenderWidget::update() {}

void RenderWidget::onClick() {}

void RenderWidget::onHover() {}

void RenderWidget::onKeyEvent() {}

void RenderWidget::show() {}

void RenderWidget::hide() {}

void RenderWidget::enable() {}

void RenderWidget::disable() {}

void RenderWidget::loadVulkan() {
	if (this->window == nullptr) {
		throw std::runtime_error("GLFWwindow is null! Cannot initialize Vulkan.");
	}
	
	vulkanModule = std::make_unique<VulkanModule>(this->window, getWidth(), getHeight(), 2);
	vulkanModule->createInstance();
	vulkanModule->setupDebugMessenger();
	vulkanModule->createSurface();
	vulkanModule->pickPhysicalDevice();
	vulkanModule->createLogicalDevice();
	vulkanModule->createSwapChain();
	vulkanModule->createImageViews();
	vulkanModule->createRenderPass();
	vulkanModule->createDescriptorSetLayout();
	vulkanModule->createGraphicsPipeline();
	vulkanModule->createCommandPool();
	vulkanModule->createColorResources();
	vulkanModule->createDepthResources();
	vulkanModule->createFrameBuffers();
	vulkanModule->createTextureImage("viking_room.png");
	vulkanModule->createTextureImageView();
	vulkanModule->createTextureSampler();
	vulkanModule->loadModel("viking_room.obj");
	vulkanModule->createVertexBuffer();
	vulkanModule->createIndexBuffer();
	vulkanModule->createUniformBuffers();
	vulkanModule->createDescriptorPool();
	vulkanModule->createDescriptorSets();
	vulkanModule->createCommandBuffers();
	vulkanModule->createSyncObjects();
}
