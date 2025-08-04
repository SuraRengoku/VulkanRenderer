#include "FileWidget.h"
#include "imgui.h"

FileWidget::FileWidget(GLFWwindow* window, uint32_t x, uint32_t y,
					   uint32_t width, uint32_t height, bool vis, bool resize,
					   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

FileWidget::~FileWidget() {}

void FileWidget::display() {}

void FileWidget::update() {}

void FileWidget::renderImGui() {
	// FileWidget完全由ImGui渲染，无Vulkan内容
	if (ImGui::Begin("File Manager", nullptr, ImGuiWindowFlags_MenuBar)) {

		// 菜单栏
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

		// 文件路径导航
		ImGui::Text("Current Path: Models/");
		ImGui::Separator();

		// 文件列表
		if (ImGui::BeginChild("FileList", ImVec2(0, -60))) {
			// 模拟文件列表
			static const char* modelFiles[] = {"viking_room.obj", "robo.obj",
											   "sphere.obj", "cube.obj"};

			static const char* textureFiles[] = {
			  "viking_room.png", "robo_a.jpg", "robo_e.jpeg", "texture.jpg"};

			// 模型文件分组
			if (ImGui::CollapsingHeader("3D Models",
										ImGuiTreeNodeFlags_DefaultOpen)) {
				for (int i = 0; i < 4; i++) {
					if (ImGui::Selectable(modelFiles[i])) {
						// loadModel(modelFiles[i]);
						ImGui::OpenPopup("Model Info");
					}

					// 右键菜单
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

			// 纹理文件分组
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

		// 底部状态栏
		ImGui::Separator();
		ImGui::Text("Status: Ready");
		ImGui::SameLine();
		if (ImGui::Button("Load Selected")) {
			// loadSelectedFile();
		}

		// 模型信息弹窗
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