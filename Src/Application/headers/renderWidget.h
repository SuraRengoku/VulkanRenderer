#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Widget.h"
#include "common.h"

class renderWidget : public Widget {
   public:
	renderWidget(GLFWwindow *window, int, int, int, int, bool, bool, WidgetStatus);
	~renderWidget();
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
	GLFWwindow *window;
};