#pragma once

#include "VulkanModule.h"
#include "Widget.h"
#include "common.h"

class FileWidget : public Widget{
   public:
	FileWidget(GLFWwindow *window, uint32_t, uint32_t, uint32_t, uint32_t, bool, bool, WidgetStatus);
	~FileWidget();
	void display() override;
	void update() override;

	void renderImGui() override;
	
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