#include "Widget.h"
#include "common.h"
#include "renderWidget.h"
#include <GLFW/glfw3.h>

renderWidget::renderWidget(GLFWwindow *window, int x, int y, int width, int height, bool vis,
						   bool resize, WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this -> window = window;
}

