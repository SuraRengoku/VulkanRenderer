#include "PropertyWidget.h"

PropertyWidget::PropertyWidget(GLFWwindow *window, uint32_t x, uint32_t y, uint32_t width,
						   uint32_t height, bool vis, bool resize,
						   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

PropertyWidget::~PropertyWidget() {
	
}

void PropertyWidget::display() {

}

void PropertyWidget::update() {}

void PropertyWidget::onClick() {}

void PropertyWidget::onHover() {}

void PropertyWidget::onKeyEvent() {}

void PropertyWidget::show() {}

void PropertyWidget::hide() {}

void PropertyWidget::enable() {}

void PropertyWidget::disable() {}