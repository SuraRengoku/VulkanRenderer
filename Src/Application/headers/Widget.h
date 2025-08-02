#pragma once

#include "common.h"

class Widget {
   public:
	Widget(int, int, int, int, bool, bool, WidgetStatus);
	virtual ~Widget() = default;
	virtual void display() = 0;
	virtual void update() {}

	virtual void onClick();
	virtual void onHover();
	virtual void onKeyEvent();

	void setPosition(int x, int y);
	void setSize(int width, int heigth);
	void setRect(int x, int y, int width, int heigth);

	virtual void show();
	virtual void hide();
	virtual void enable();
	virtual void disable();
	
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;
	bool visibility() const;
	bool resizibility() const;
	WidgetStatus getStatus() const;
   private:
	int x, y; // coordinates
	int width;
	int height;
	bool visible;
	bool resizeable;
	WidgetStatus status;
};