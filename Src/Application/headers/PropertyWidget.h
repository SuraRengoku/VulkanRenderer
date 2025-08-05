#pragma once

#include <memory>

#include "VulkanModule.h"
#include "Widget.h"
#include "common.h"

class PropertyWidget : public Widget {
   public:
	PropertyWidget(GLFWwindow* window, uint32_t, uint32_t, uint32_t, uint32_t,
				   bool, bool, WidgetStatus);
	PropertyWidget(GLFWwindow* window, WLayout, bool, bool, WidgetStatus);
	~PropertyWidget();
	void display() override;
	void update() override;

	void renderImGui() override;

	const char* getTypeName() const override { return "PropertyWidget"; }

	void onClick() override;
	void onHover() override;
	void onKeyEvent() override;

	void show() override;
	void hide() override;
	void enable() override;
	void disable() override;

   private:
	GLFWwindow* window;

	std::unique_ptr<Loader>
	  selectedObject;  // TODO waiting to be replaced by Object
};