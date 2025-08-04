#pragma once

#include <memory>
#include <vector>
#include "renderWidget.h"
#include "FileWidget.h"
#include "PropertyWidget.h"
#include "Widget.h"

#define __DEBUG__

class Application {
   public:
	Application(uint32_t x = 400, uint32_t y = 300, uint32_t width = 800, uint32_t height = 600);
	~Application();

	void initWindow(const std::string& title);
	void runApplicaton();
	void terminateWindow();
	
	// ImGui 
	void initImGui();
	void renderImGui();
	void renderImGuiSoftware();
	void processImGuiFrame();
	void cleanupImGui();
	
	// ImGui辅助渲染方法
	void renderCoordinateAxes();
	void renderPerformanceOverlay();
	
	// Vulkan辅助方法
	VkCommandBuffer beginSingleTimeCommands(VulkanModule* vulkanModule);
	void endSingleTimeCommands(VulkanModule* vulkanModule, VkCommandBuffer commandBuffer);

   private:
	static void ApplicationResizedCallback(GLFWwindow *window, int width, int height);

	uint32_t X;
	uint32_t Y;
	uint32_t WIDTH;
	uint32_t HEIGHT;
	GLFWwindow *window;
	bool ApplicationWindowResized = false;

	std::shared_ptr<Scene> scene;
	std::vector<std::unique_ptr<Widget>> widgets;  
	
	// quick get (for ImGUI)
	RenderWidget* renderWidget = nullptr;
	FileWidget* fileWidget = nullptr;
	PropertyWidget* propertyWidget = nullptr;
	
	// ImGui Vulkan资源
	VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;

};