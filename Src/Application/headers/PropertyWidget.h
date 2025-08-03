#pragma once

#include "VulkanModule.h"
#include "Widget.h"
#include "common.h"

class PropertyWidget : public Widget {
   public:
	PropertyWidget(GLFWwindow *window, uint32_t, uint32_t, uint32_t, uint32_t, bool, bool, WidgetStatus);
	~PropertyWidget();
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