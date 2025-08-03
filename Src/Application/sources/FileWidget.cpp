#include "FileWidget.h"

FileWidget::FileWidget(GLFWwindow *window, uint32_t x, uint32_t y, uint32_t width,
						   uint32_t height, bool vis, bool resize,
						   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

FileWidget::~FileWidget() {
	
}

void FileWidget::display() {

}

void FileWidget::update() {}

void FileWidget::onClick() {}

void FileWidget::onHover() {}

void FileWidget::onKeyEvent() {}

void FileWidget::show() {}

void FileWidget::hide() {}

void FileWidget::enable() {}

void FileWidget::disable() {}