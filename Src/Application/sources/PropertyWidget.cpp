#include "PropertyWidget.h"
#include "imgui.h"

PropertyWidget::PropertyWidget(GLFWwindow* window, uint32_t x, uint32_t y,
							   uint32_t width, uint32_t height, bool vis,
							   bool resize, WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

PropertyWidget::PropertyWidget(GLFWwindow* window, WLayout layout, bool vis,
							   bool resize, WidgetStatus status)
	: PropertyWidget(window, layout.x, layout.y, layout.width, layout.height,
					 vis, resize, status) {}

PropertyWidget::~PropertyWidget() {}

void PropertyWidget::display() {}

void PropertyWidget::update() {}

void PropertyWidget::renderImGui() {
	ImVec2 windowPos = ImVec2(this->getX(), this->getY());
	ImVec2 windowSize = ImVec2(this->getWidth(), this->getHeight());

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	if (ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Object")) {
				if (ImGui::MenuItem("Reset Transform")) {
					// resetTransform();
				}
				if (ImGui::MenuItem("Duplicate")) {
					// duplicateObject();
				}
				if (ImGui::MenuItem("Delete")) {
					// deleteObject();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform",
									ImGuiTreeNodeFlags_DefaultOpen)) {
			static float position[3] = {0.0f, 0.0f, 0.0f};
			static float rotation[3] = {0.0f, 0.0f, 0.0f};
			static float scale[3] = {1.0f, 1.0f, 1.0f};

			ImGui::DragFloat3("Position", position, 0.1f);
			ImGui::DragFloat3("Rotation", rotation, 1.0f, -180.0f, 180.0f);
			ImGui::DragFloat3("Scale", scale, 0.01f, 0.01f, 10.0f);

			if (ImGui::Button("Reset Transform")) {
				position[0] = position[1] = position[2] = 0.0f;
				rotation[0] = rotation[1] = rotation[2] = 0.0f;
				scale[0] = scale[1] = scale[2] = 1.0f;
			}
		}

		if (ImGui::CollapsingHeader("Material",
									ImGuiTreeNodeFlags_DefaultOpen)) {
			static float albedo[3] = {0.8f, 0.8f, 0.8f};
			static float metallic = 0.0f;
			static float roughness = 0.5f;
			static float emission[3] = {0.0f, 0.0f, 0.0f};

			ImGui::ColorEdit3("Albedo", albedo);
			ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
			ImGui::ColorEdit3("Emission", emission);

			ImGui::Separator();
			ImGui::Text("Textures:");
			if (ImGui::Button("Load Diffuse Texture")) {
				// loadDiffuseTexture();
			}
			if (ImGui::Button("Load Normal Map")) {
				// loadNormalMap();
			}
		}

		if (ImGui::CollapsingHeader("Lighting")) {
			static float lightColor[3] = {1.0f, 1.0f, 1.0f};
			static float intensity = 1.0f;
			static float range = 10.0f;

			ImGui::ColorEdit3("Light Color", lightColor);
			ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f);
			ImGui::SliderFloat("Range", &range, 0.1f, 100.0f);
		}

		if (ImGui::CollapsingHeader("Camera")) {
			static float fov = 45.0f;
			static float nearPlane = 0.1f;
			static float farPlane = 100.0f;

			ImGui::SliderFloat("Field of View", &fov, 10.0f, 120.0f);
			ImGui::DragFloat("Near Plane", &nearPlane, 0.01f, 0.01f, 10.0f);
			ImGui::DragFloat("Far Plane", &farPlane, 1.0f, 1.0f, 1000.0f);
		}
	}
	ImGui::End();
}

void PropertyWidget::onClick() {}

void PropertyWidget::onHover() {}

void PropertyWidget::onKeyEvent() {}

void PropertyWidget::show() {}

void PropertyWidget::hide() {}

void PropertyWidget::enable() {}

void PropertyWidget::disable() {}