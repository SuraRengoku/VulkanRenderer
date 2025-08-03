#include "Application.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "renderWidget.h"

Application::Application(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	: X(x), Y(y), WIDTH(width), HEIGHT(height) {
    initWindow("VulkanRenderer");
}

Application::~Application() {
    terminateWindow();
}

void Application::initWindow(const std::string& title) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    this -> window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, ApplicationResizedCallback);
}

void Application::ApplicationResizedCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app -> ApplicationWindowResized  = true;
}

void Application::runApplicaton() {
    // TODO change size
    RenderWidget renderWidget(window, X, Y, WIDTH, HEIGHT, true, true, WidgetStatus::ACTIVATED);
    // TODO FileWidget
    // TODO PropertyWdiget
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderWidget.display();
    }
}

void Application::terminateWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}