#include <GLFW/glfw3.h>
#include <_stdio.h>

#include "FileWidget.h"
#include "Widget.h"
#include "common.h"
#include "imgui.h"

FileWidget::FileWidget(GLFWwindow* window, uint32_t x, uint32_t y,
					   uint32_t width, uint32_t height, bool vis, bool resize,
					   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

FileWidget::FileWidget(GLFWwindow* window, WLayout layout, bool vis,
					   bool resize, WidgetStatus status)
	: FileWidget(window, layout.x, layout.y, layout.width, layout.height, vis,
				 resize, status) {}

FileWidget::~FileWidget() {}

void FileWidget::display() {}

void FileWidget::update() {}

void FileWidget::renderImGui() {
	ImVec2 windowPos = ImVec2(this->getX(), this->getY());
	ImVec2 windowSize = ImVec2(this->getWidth(), this->getHeight());

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	if (ImGui::Begin("File Manager", nullptr, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Import Model...")) {
					// openFileDialog();
				}
				if (ImGui::MenuItem("Import Texture...")) {
					// openTextureDialog();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Refresh")) {
					// refreshFileList();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("Current Path: Models/");
		ImGui::Separator();

		if (ImGui::BeginChild("FileList", ImVec2(0, -60))) {
			static const char* modelFiles[] = {"viking_room.obj", "robo.obj",
											   "sphere.obj", "cube.obj"};

			static const char* textureFiles[] = {
			  "viking_room.png", "robo_a.jpg", "robo_e.jpeg", "texture.jpg"};

			if (ImGui::CollapsingHeader("3D Models",
										ImGuiTreeNodeFlags_DefaultOpen)) {
				for (int i = 0; i < 4; i++) {
					if (ImGui::Selectable(modelFiles[i])) {
						// loadModel(modelFiles[i]);
						ImGui::OpenPopup("Model Info");
					}

					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::MenuItem("Load Model")) {
							// loadModel(modelFiles[i]);
						}
						if (ImGui::MenuItem("Show Properties")) {
							// showModelProperties(modelFiles[i]);
						}
						ImGui::EndPopup();
					}
				}
			}

			if (ImGui::CollapsingHeader("Textures",
										ImGuiTreeNodeFlags_DefaultOpen)) {
				for (int i = 0; i < 4; i++) {
					if (ImGui::Selectable(textureFiles[i])) {
						// loadTexture(textureFiles[i]);
					}

					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::MenuItem("Apply Texture")) {
							// applyTexture(textureFiles[i]);
						}
						if (ImGui::MenuItem("Preview")) {
							// previewTexture(textureFiles[i]);
						}
						ImGui::EndPopup();
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::Separator();
		ImGui::Text("Status: Ready");
		ImGui::SameLine();
		if (ImGui::Button("Load Selected")) {
			// loadSelectedFile();
		}

		if (ImGui::BeginPopupModal("Model Info", nullptr,
								   ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Model Information");
			ImGui::Separator();
			ImGui::Text("Vertices: 1234");
			ImGui::Text("Faces: 2468");
			ImGui::Text("Materials: 1");

			if (ImGui::Button("Load")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void FileWidget::onClick() {}

void FileWidget::onHover() {}

void FileWidget::onKeyEvent() {}

void FileWidget::show() {}

void FileWidget::hide() {}

void FileWidget::enable() {}

void FileWidget::disable() {}