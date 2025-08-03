#pragma once

#include <memory>
#include "GLFW/glfw3.h"
#include "VulkanModule.h"
#include "Widget.h"
#include "common.h"

#define __USE_VULKAN__

class RenderWidget : public Widget {
   public:
	RenderWidget(GLFWwindow *window, uint32_t, uint32_t, uint32_t, uint32_t, bool, bool, WidgetStatus);
	~RenderWidget();
	void display() override;
	void update() override;
	
	void onClick() override;
	void onHover() override;
	void onKeyEvent() override;

	void show() override;
	void hide() override;
	void enable() override;
	void disable() override;

   private:
	void loadVulkan();
	GLFWwindow *window;
};