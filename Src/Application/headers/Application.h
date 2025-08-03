#pragma once

#include "renderWidget.h"
#include "FileWidget.h"
#include "PropertyWidget.h"

class Application {
   public:
	Application(uint32_t x = 400, uint32_t y = 300, uint32_t width = 800, uint32_t height = 600);
	~Application();

	void initWindow(const std::string& title);
	void runApplicaton();
	void terminateWindow();

   private:
	static void ApplicationResizedCallback(GLFWwindow *window, int width, int height);

	uint32_t X;
	uint32_t Y;
	uint32_t WIDTH;
	uint32_t HEIGHT;
	GLFWwindow *window;
	bool ApplicationWindowResized = false;
};