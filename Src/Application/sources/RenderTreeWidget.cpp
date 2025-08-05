#include "RenderTreeWidget.h"
#include "imgui.h"

RenderTreeWidget::RenderTreeWidget(GLFWwindow *window, uint32_t x, uint32_t y,
								   uint32_t width, uint32_t height, bool vis,
								   bool resize, WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

RenderTreeWidget::RenderTreeWidget(GLFWwindow* window, WLayout layout, bool vis,
					   bool resize, WidgetStatus status)
	: RenderTreeWidget(window, layout.x, layout.y, layout.width, layout.height, vis,
				 resize, status) {}

RenderTreeWidget::~RenderTreeWidget() {}

void RenderTreeWidget::display() {}

void RenderTreeWidget::update() {}

void RenderTreeWidget::renderImGui() {
	ImVec2 windowPos = ImVec2(this->getX(), this->getY());
	ImVec2 windowSize = ImVec2(this->getWidth(), this->getHeight());

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	if(ImGui::Begin("RenderTree", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::CollapsingHeader("Scene Hierarchy",
									ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::TreeNode("Scene Root")) {
				if (ImGui::TreeNode("Models")) {
					if (ImGui::Selectable("Viking Room", false)) {
						// selectObject("Viking Room");
					}
					if (ImGui::Selectable("Robot", false)) {
						// selectObject("Robot");
					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Lights")) {
					if (ImGui::Selectable("Directional Light", false)) {
						// selectObject("Directional Light");
					}
					if (ImGui::Selectable("Point Light", false)) {
						// selectObject("Point Light");
					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Camera")) {
					if (ImGui::Selectable("Main Camera", false)) {
						// selectObject("Main Camera");
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void RenderTreeWidget::onClick() {}

void RenderTreeWidget::onHover() {}

void RenderTreeWidget::onKeyEvent() {}

void RenderTreeWidget::show() {}

void RenderTreeWidget::hide() {}

void RenderTreeWidget::enable() {}

void RenderTreeWidget::disable() {}