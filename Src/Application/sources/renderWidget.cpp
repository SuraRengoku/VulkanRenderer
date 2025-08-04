#include <cstdio>
#include <exception>
#include <stdexcept>
#include <string>

#include "RenderWidget.h"
#include "imgui.h"

RenderWidget::RenderWidget(GLFWwindow *window, uint32_t x, uint32_t y,
						   uint32_t width, uint32_t height, bool vis,
						   bool resize, WidgetStatus status)
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

RenderWidget::~RenderWidget() {
	if (vulkanModule) {
		vulkanModule->cleanup();
	}
}

void RenderWidget::display() {
	// 纯Vulkan 3D场景渲染，不包含任何UI元素
	vulkanModule->drawFrame();
}

void RenderWidget::update() {
	// 更新3D场景状态
}

void RenderWidget::renderImGui() {
	// RenderWidget的ImGui界面：仅渲染设置控制面板
	if (ImGui::Begin("Render Settings")) {
		ImGui::Text("3D Rendering Controls");
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Display Settings")) {
			ImGui::Checkbox("Wireframe Mode", &wireframeMode);
			ImGui::SliderFloat("FOV", &fov, 10.0f, 120.0f);
			ImGui::ColorEdit3("Background Color", backgroundColor);
		}

		if (ImGui::CollapsingHeader("Camera Controls")) {
			if (ImGui::Button("Reset Camera")) {
				// resetCamera();
			}

			// 可以添加更多相机控制
			static float cameraSpeed = 1.0f;
			ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.1f, 5.0f);
		}

		if (ImGui::CollapsingHeader("Lighting")) {
			static float lightIntensity = 1.0f;
			static float ambientStrength = 0.1f;

			ImGui::SliderFloat("Light Intensity", &lightIntensity, 0.0f, 3.0f);
			ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f,
							   1.0f);
		}
	}
	ImGui::End();
}

void RenderWidget::onClick() {}

void RenderWidget::onHover() {}

void RenderWidget::onKeyEvent() {}

void RenderWidget::show() {}

void RenderWidget::hide() {}

void RenderWidget::enable() {}

void RenderWidget::disable() {}

void RenderWidget::loadVulkan() {
	if (this->window == nullptr) {
		throw std::runtime_error(
		  "Application::RenderWidget::loadVulkan: GLFWwindow is null! Cannot "
		  "initialize Vulkan.");
	}

	try {
		vulkanModule = std::make_unique<VulkanModule>(this->window, getWidth(),
													  getHeight(), 2);
	} catch (const std::exception &e) {
		std::string errorMsg = "Application::RenderWidget::loadVulkan: ";
		errorMsg += e.what();
		throw std::runtime_error(errorMsg);
	}

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
