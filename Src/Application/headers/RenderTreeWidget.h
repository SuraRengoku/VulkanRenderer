#pragma once

#include <future>
#include <memory>

#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Widget.h"
#include "common.h"

class RenderTreeWidget : public Widget {
   public:
	RenderTreeWidget(GLFWwindow* window, uint32_t, uint32_t, uint32_t, uint32_t,
					 bool, bool, WidgetStatus);
	RenderTreeWidget(GLFWwindow* window, WLayout layout, bool, bool,
					 WidgetStatus);
	~RenderTreeWidget();
	void display() override;
	void update() override;

	void renderImGui() override;

	const char* getTypeName() const override { return "RenderTreeWidget"; }

	void onClick() override;
	void onHover() override;
	void onKeyEvent() override;

	void show() override;
	void hide() override;
	void enable() override;
	void disable() override;

   private:
	GLFWwindow* window;
	std::shared_ptr<Scene> scene;
};