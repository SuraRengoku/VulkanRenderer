#include "Widget.h"
#include "common.h"

Widget::Widget(int x, int y, int width, int height, bool vis, bool resize,
			   WidgetStatus status)
	: x(x),
	  y(y),
	  width(width),
	  height(height),
	  visible(vis),
	  resizeable(resize),
	  status(status) {}

void Widget::setPosition(int x, int y) {
	this -> x = x;
	this -> y = y;
}

void Widget::setSize(int width, int heigth) {
	this -> width = width;
	this -> height = height;
}

void Widget::setRect(int x, int y, int width, int heigth) {
	setPosition(x, y);
	setSize(width, heigth);
}

int Widget::getX() const { return x; }
int Widget::getY() const { return y; }
int Widget::getWidth() const { return width; }
int Widget::getHeight() const { return height; }
bool Widget::visibility() const { return visible; }
bool Widget::resizibility() const { return resizeable; }
WidgetStatus Widget::getStatus() const { return status; }