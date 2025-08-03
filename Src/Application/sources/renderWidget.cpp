#include "RenderWidget.h"
#include <cstdio>

RenderWidget::RenderWidget(GLFWwindow *window, uint32_t x, uint32_t y, uint32_t width,
						   uint32_t height, bool vis, bool resize,
						   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
#if defined(__USE_VULKAN__)
	printf("load Vulkan module");
	loadVulkan();
#elif defined(__USE_OPENGL__)
#elif defined(__USE_DX11__)
#elif defined(__USE_DX12__)
#endif
}

RenderWidget::~RenderWidget() {}

void RenderWidget::display() {

}

void RenderWidget::update() {}

void RenderWidget::onClick() {}

void RenderWidget::onHover() {}

void RenderWidget::onKeyEvent() {}

void RenderWidget::show() {}

void RenderWidget::hide() {}

void RenderWidget::enable() {}

void RenderWidget::disable() {}

void RenderWidget::loadVulkan() {
	
}
