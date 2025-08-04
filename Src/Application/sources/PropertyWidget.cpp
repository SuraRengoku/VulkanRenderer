#include "PropertyWidget.h"
#include "imgui.h"

PropertyWidget::PropertyWidget(GLFWwindow *window, uint32_t x, uint32_t y, uint32_t width,
						   uint32_t height, bool vis, bool resize,
						   WidgetStatus status)
	: Widget(x, y, width, height, vis, resize, status) {
	this->window = window;
}

PropertyWidget::~PropertyWidget() {
	
}

void PropertyWidget::display() {

}

void PropertyWidget::update() {}

void PropertyWidget::renderImGui() {
	// PropertyWidget完全由ImGui渲染，提供详细的对象属性编辑界面
	if (ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_MenuBar)) {
		
		// 菜单栏
		if(ImGui::BeginMenuBar()) {
			if(ImGui::BeginMenu("Object")) {
				if(ImGui::MenuItem("Reset Transform")) {
					// resetTransform();
				}
				if(ImGui::MenuItem("Duplicate")) {
					// duplicateObject();
				}
				if(ImGui::MenuItem("Delete")) {
					// deleteObject();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
		// 场景层次结构
		if(ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen)) {
			// 模拟场景对象树
			if(ImGui::TreeNode("Scene Root")) {
				if(ImGui::TreeNode("Models")) {
					if(ImGui::Selectable("Viking Room", false)) {
						// selectObject("Viking Room");
					}
					if(ImGui::Selectable("Robot", false)) {
						// selectObject("Robot");
					}
					ImGui::TreePop();
				}
				
				if(ImGui::TreeNode("Lights")) {
					if(ImGui::Selectable("Directional Light", false)) {
						// selectObject("Directional Light");
					}
					if(ImGui::Selectable("Point Light", false)) {
						// selectObject("Point Light");
					}
					ImGui::TreePop();
				}
				
				if(ImGui::TreeNode("Camera")) {
					if(ImGui::Selectable("Main Camera", false)) {
						// selectObject("Main Camera");
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		
		ImGui::Separator();
		
		// 选中对象的属性
		if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			static float position[3] = {0.0f, 0.0f, 0.0f};
			static float rotation[3] = {0.0f, 0.0f, 0.0f};
			static float scale[3] = {1.0f, 1.0f, 1.0f};
			
			ImGui::DragFloat3("Position", position, 0.1f);
			ImGui::DragFloat3("Rotation", rotation, 1.0f, -180.0f, 180.0f);
			ImGui::DragFloat3("Scale", scale, 0.01f, 0.01f, 10.0f);
			
			if(ImGui::Button("Reset Transform")) {
				position[0] = position[1] = position[2] = 0.0f;
				rotation[0] = rotation[1] = rotation[2] = 0.0f;
				scale[0] = scale[1] = scale[2] = 1.0f;
			}
		}
		
		// 材质属性
		if(ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
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
			if(ImGui::Button("Load Diffuse Texture")) {
				// loadDiffuseTexture();
			}
			if(ImGui::Button("Load Normal Map")) {
				// loadNormalMap();
			}
		}
		
		// 光照属性（当选中光源时显示）
		if(ImGui::CollapsingHeader("Lighting")) {
			static float lightColor[3] = {1.0f, 1.0f, 1.0f};
			static float intensity = 1.0f;
			static float range = 10.0f;
			
			ImGui::ColorEdit3("Light Color", lightColor);
			ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f);
			ImGui::SliderFloat("Range", &range, 0.1f, 100.0f);
		}
		
		// 相机属性（当选中相机时显示）
		if(ImGui::CollapsingHeader("Camera")) {
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