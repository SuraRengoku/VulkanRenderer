#pragma once

#include <memory>
#include <vector>

#include "FileWidget.h"
#include "PropertyWidget.h"
#include "RenderWidget.h"
#include "RenderTreeWidget.h"
#include "Widget.h"

#define __DEBUG__

class Application {
   public:
	Application(uint32_t x = 400, uint32_t y = 300, uint32_t width = 800,
				uint32_t height = 600);
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

	// Vulkan Auxiliary
	VkCommandBuffer beginSingleTimeCommands(VulkanModule* vulkanModule);
	void endSingleTimeCommands(VulkanModule* vulkanModule,
							   VkCommandBuffer commandBuffer);

   private:
	static void ApplicationResizedCallback(GLFWwindow* window, int width,
										   int height);

	inline void debugWidgetPosSize(Widget* widget);

	uint32_t X;
	uint32_t Y;
	uint32_t WIDTH;
	uint32_t HEIGHT;
	WLayout layout;
	GLFWwindow* window;
	bool ApplicationWindowResized = false;

	std::shared_ptr<Scene> scene;
	std::vector<std::unique_ptr<Widget>> widgets;

	// quick get (for ImGUI)
	RenderWidget* renderWidget = nullptr;
	RenderTreeWidget* renderTreeWidget = nullptr;
	FileWidget* fileWidget = nullptr;
	PropertyWidget* propertyWidget = nullptr;

	// widget layout
	WLayoutScale renderLayoutScale;
	WLayoutScale renderTreeLayoutScale;
	WLayoutScale fileLayoutScale;
	WLayoutScale PropertyLayoutScale;

	// ImGui Vulkan Resources
	VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;
};