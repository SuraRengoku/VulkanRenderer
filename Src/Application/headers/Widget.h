#pragma once

#include <cstdint>

#include "common.h"

struct WLayoutScale {
	float xs;
	float ys;
	float widths;
	float heights;
	float s;
};

struct WLayout {
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
};

inline WLayout scalingLayout(WLayout layout, WLayoutScale scale) {
	return {static_cast<uint32_t>(layout.width * scale.xs * scale.s),
			static_cast<uint32_t>(layout.height * scale.ys * scale.s),
			static_cast<uint32_t>(layout.width * scale.widths * scale.s),
			static_cast<uint32_t>(layout.height * scale.heights * scale.s)};
}

class Widget {
   public:
	Widget(uint32_t, uint32_t, uint32_t, uint32_t, bool, bool, WidgetStatus);
	Widget(WLayout, bool, bool, WidgetStatus);
	virtual ~Widget() = default;
	virtual void display() = 0;
	virtual void update() = 0;

	virtual void renderImGui() {}

	virtual const char* getTypeName() const = 0;

	virtual void onClick() = 0;
	virtual void onHover() = 0;
	virtual void onKeyEvent() = 0;

	void setPosition(uint32_t x, uint32_t y);
	void setSize(uint32_t width, uint32_t height);
	void setRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	virtual void show() = 0;
	virtual void hide() = 0;
	virtual void enable() = 0;
	virtual void disable() = 0;

	uint32_t getX() const;
	uint32_t getY() const;
	uint32_t getWidth() const;
	uint32_t getHeight() const;
	bool visibility() const;
	bool resizibility() const;
	WidgetStatus getStatus() const;

   private:
	uint32_t x, y;	// coordinates
	uint32_t width;
	uint32_t height;
	float scale = 1.0f;
	bool visible;
	bool resizeable;
	WidgetStatus status;
};