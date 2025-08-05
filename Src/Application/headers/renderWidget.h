#pragma once

#include <memory>

#include "GLFW/glfw3.h"
#include "Scene.h"
#include "VulkanModule.h"
#include "Widget.h"
#include "common.h"

#define __USE_VULKAN__

class RenderWidget : public Widget {
   public:
	RenderWidget(GLFWwindow* window, uint32_t, uint32_t, uint32_t, uint32_t,
				 bool, bool, WidgetStatus);
	RenderWidget(GLFWwindow* window, WLayout, bool, bool, WidgetStatus);
	~RenderWidget();
	void display() override;
	void update() override;

	void renderImGui() override;
	void renderCoordinateAxes();
	void renderPerformanceOverlay();

	const char* getTypeName() const override { return "RenderWidget"; }

	bool& getWireframeMode() { return wireframeMode; }
	float& getFOV() { return fov; }

	// Vulkan getter functions（for ImGui integrated）
	VulkanModule* getVulkanModule() { return vulkanModule.get(); }

	void onClick() override;
	void onHover() override;
	void onKeyEvent() override;

	void show() override;
	void hide() override;
	void enable() override;
	void disable() override;

   private:
	void loadVulkan();
	GLFWwindow* window;
	std::unique_ptr<VulkanModule> vulkanModule;
	std::shared_ptr<Scene> scene;

	bool wireframeMode = false;
	float fov = 45.0f;
	float backgroundColor[3] = {0.0f, 0.0f,
								0.0f};	// RGB values for background color
};