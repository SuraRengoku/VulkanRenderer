#include "Widget.h"
#include "common.h"

Widget::Widget(uint32_t x, uint32_t y, uint32_t width, uint32_t height, bool vis, bool resize,
			   WidgetStatus status)
	: x(x),
	  y(y),
	  width(width),
	  height(height),
	  visible(vis),
	  resizeable(resize),
	  status(status) {

}

void Widget::setPosition(uint32_t x, uint32_t y) {
	this -> x = x;
	this -> y = y;
}

void Widget::setSize(uint32_t width, uint32_t height) {
	this -> width = width;
	this -> height = height;
}

void Widget::setRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	setPosition(x, y);
	setSize(width, height);
}

uint32_t Widget::getX() const { return x; }
uint32_t Widget::getY() const { return y; }
uint32_t Widget::getWidth() const { return width; }
uint32_t Widget::getHeight() const { return height; }
bool Widget::visibility() const { return visible; }
bool Widget::resizibility() const { return resizeable; }
WidgetStatus Widget::getStatus() const { return status; }