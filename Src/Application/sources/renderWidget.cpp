#include <cstdio>
#include <exception>
#include <stdexcept>
#include <string>

#include "RenderWidget.h"
#include "Widget.h"
#include "common.h"
#include "imgui.h"

RenderWidget::RenderWidget(GLFWwindow* window, uint32_t x, uint32_t y,
						   uint32_t width, uint32_t height, bool vis,
						   bool resize, WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
#if defined(__USE_VULKAN__)
	printf("loading Vulkan module......\n");
	loadVulkan();
#elif defined(__USE_OPENGL__)
	printf("loading OpenGL module.....n");
#elif defined(__WIN32__) && definde(__USE_DX11__)
#elif defined(__WIN32__) && defined(__USE_DX12__)
#endif
}

RenderWidget::RenderWidget(GLFWwindow* window, WLayout layout, bool vis,
						   bool resize, WidgetStatus status)
	: RenderWidget(window, layout.x, layout.y, layout.width, layout.height, vis,
				   resize, status) {}

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
	// ImVec2 windowPos = ImVec2(this->getX(), this->getY());
	// ImVec2 windowSize = ImVec2(this->getWidth(), this->getHeight());

	// ImGui::SetNextWindowPos(windowPos);
	// ImGui::SetNextWindowSize(windowSize);

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

			// camera controlling
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

	this->renderCoordinateAxes();
	this->renderPerformanceOverlay();
	ImGui::End();
}

void RenderWidget::renderCoordinateAxes() {
	// left bottom corner
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowPos = ImVec2(50, io.DisplaySize.y - 100);
	ImVec2 windowSize = ImVec2(80, 80);

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowBgAlpha(1.0f);

	if (ImGui::Begin(
		  "Coordinate Axes", nullptr,
		  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground)) {

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 center = ImVec2(windowPos.x + 40, windowPos.y + 40);

		// X axis (red)
		drawList->AddLine(center, ImVec2(center.x + 30, center.y),
						  IM_COL32(255, 0, 0, 255), 2.0f);
		drawList->AddText(ImVec2(center.x + 32, center.y - 8),
						  IM_COL32(255, 0, 0, 255), "X");

		// Y axis (green)
		drawList->AddLine(center, ImVec2(center.x, center.y - 30),
						  IM_COL32(0, 255, 0, 255), 2.0f);
		drawList->AddText(ImVec2(center.x + 2, center.y - 32),
						  IM_COL32(0, 255, 0, 255), "Y");

		// Z axis (blue) - depth
		drawList->AddLine(center, ImVec2(center.x - 20, center.y + 20),
						  IM_COL32(0, 0, 255, 255), 2.0f);
		drawList->AddText(ImVec2(center.x - 25, center.y + 22),
						  IM_COL32(0, 0, 255, 255), "Z");
	}
	ImGui::End();
}

void RenderWidget::renderPerformanceOverlay() {
	// right top corner
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowPos = ImVec2(io.DisplaySize.x - 200, 25);

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowBgAlpha(0.3f);

	if (ImGui::Begin(
		  "Performance", nullptr,
		  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBackground)) {

		ImGui::Text("FPS: %.1f", io.Framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

		// performance information
		ImGui::Text("Render Mode: %s",
					this->getWireframeMode() ? "Wireframe" : "Solid");
		ImGui::Text("FOV: %.1f°", this->getFOV());
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
		vulkanModule = std::make_unique<VulkanModule>(
		  this->window, 800, 600, 2, this->getWidth() + 800,
		  this->getHeight() + 600, this->getX(), this->getY());
	} catch (const std::exception& e) {
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
