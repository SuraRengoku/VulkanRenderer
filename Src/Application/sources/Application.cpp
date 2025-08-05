#include <memory>
#include <typeinfo>

#include "Application.h"
#include "FileWidget.h"
#include "GLFW/glfw3.h"
#include "PropertyWidget.h"
#include "RenderTreeWidget.h"
#include "RenderWidget.h"
#include "Widget.h"
#include "common.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

inline void Application::debugWidgetPosSize(Widget* widget) {
	std::cout << widget->getTypeName() << " getX: " << widget->getX()
			  << " getY: " << widget->getY()
			  << " getWidth: " << widget->getWidth()
			  << " getHeight: " << widget->getHeight() << std::endl;
}

Application::Application(uint32_t x, uint32_t y, uint32_t width,
						 uint32_t height)
	: X(x), Y(y), WIDTH(width), HEIGHT(height) {
	this->layout = {x, y, width, height};

	renderLayoutScale = {0.0f, 0.0f, 0.75f, 0.75f, 1.0f};
	fileLayoutScale = {0.0f, 0.75f, 0.75f, 0.25f, 1.0f};
	renderTreeLayoutScale = {0.75f, 0.0f, 0.25f, 0.5f, 1.0f};
	PropertyLayoutScale = {0.75f, 0.5f, 0.25f, 0.5f, 1.0f};

	initWindow("VulkanRenderer");
}

Application::~Application() { terminateWindow(); }

void Application::initWindow(const std::string& title) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	this->window =
	  glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, ApplicationResizedCallback);
}

void Application::ApplicationResizedCallback(GLFWwindow* window, int width,
											 int height) {
	auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->ApplicationWindowResized = true;
}

void Application::runApplicaton() {
	// create shared scene
	scene = std::make_shared<Scene>();

	// create Rendering Widget（Vulkan 3D）
	auto renderWidgetPtr = std::make_unique<RenderWidget>(
	  window, scalingLayout(layout, renderLayoutScale), true, true,
	  WidgetStatus::ACTIVATED);
	renderWidget = renderWidgetPtr.get();
	widgets.push_back(std::move(renderWidgetPtr));

	// create File Widget（ImGui）
	auto fileWidgetPtr = std::make_unique<FileWidget>(
	  window, scalingLayout(layout, fileLayoutScale), true, true,
	  WidgetStatus::ACTIVATED);
	fileWidget = fileWidgetPtr.get();
	widgets.push_back(std::move(fileWidgetPtr));

	// create File Widget（ImGui）
	auto propertyWidgetPtr = std::make_unique<PropertyWidget>(
	  window, scalingLayout(layout, PropertyLayoutScale), true, true,
	  WidgetStatus::ACTIVATED);
	propertyWidget = propertyWidgetPtr.get();
	widgets.push_back(std::move(propertyWidgetPtr));

	auto renderTreeWidgetPtr = std::make_unique<RenderTreeWidget>(
	  window, scalingLayout(layout, renderTreeLayoutScale), true, true,
	  WidgetStatus::ACTIVATED);
	renderTreeWidget = renderTreeWidgetPtr.get();
	widgets.push_back(std::move(renderTreeWidgetPtr));

#if defined(__DEBUG__)
	for (auto& widget : widgets) {
		debugWidgetPosSize(widget.get());
	}
#endif

	// before initialize ImGui, make sure Vulkan is entirely initialized
	renderWidget->getVulkanModule()->waitDeviceIdle();
	// initialize ImGui using Vulkan backend
	initImGui();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		renderWidget->display();

		renderImGui();

		for (auto& widget : widgets) {
			widget->update();
		}

		glfwSwapBuffers(window);
	}

	// make sure all Vulkan Devices are idle before cleaning ImGui
	if (renderWidget) {
		renderWidget->getVulkanModule()->waitDeviceIdle();
	}
	cleanupImGui();
}

void Application::terminateWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::initImGui() {
	std::cout << std::string("IMGUI CHECK: ") +
				   (IMGUI_CHECKVERSION() ? "success" : "failed")
			  << std::endl;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan(window, true);

	io.DisplaySize = ImVec2((float)WIDTH, (float)HEIGHT);
#if defined(__APPLE__)
	io.DisplayFramebufferScale = ImVec2(2.0f, 2.0f);  // for Retina displaying
#elif defined(__WIN32__)
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
#endif

	VulkanModule* vulkanModule = renderWidget->getVulkanModule();

	// specified descriptor pool for ImGui, Vulkan docs recommand
	VkDescriptorPoolSize poolSizes[] = {
	  {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
	  {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
	  {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
	  {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
	  {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
	  {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
	  {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
	  {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
	  {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
	  {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
	  {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = 1000;
	poolInfo.poolSizeCount = std::size(poolSizes);
	poolInfo.pPoolSizes = poolSizes;

	if (vkCreateDescriptorPool(vulkanModule->getDevice(), &poolInfo, nullptr,
							   &imguiDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error(
		  "Application::Application::initImGui: failed to create ImGui "
		  "descriptor pool!");
	}

	// ImGui Vulkan initializing configuration
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = vulkanModule->getInstance();
	init_info.PhysicalDevice = vulkanModule->getPhysicalDevice();
	init_info.Device = vulkanModule->getDevice();
	init_info.QueueFamily = vulkanModule->getGraphicsQueueFamily();
	init_info.Queue = vulkanModule->getGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = imguiDescriptorPool;
	init_info.RenderPass = vulkanModule->getRenderPass();
	init_info.Subpass = 0;
	init_info.MinImageCount = 2;
	init_info.ImageCount = vulkanModule->getSwapChainImageCount();
	init_info.MSAASamples = vulkanModule->getMSAASamples();
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info);
	ImGui_ImplVulkan_CreateFontsTexture();
}

void Application::renderImGui() {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Import Model")) {
				// fileWidget->openFileDialog();
			}
			if (ImGui::MenuItem("Export Scene")) {
				// export scene
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {
			static bool showFilePanel = true;
			static bool showPropertyPanel = true;
			static bool showCoordinateAxes = true;

			ImGui::MenuItem("File Panel", nullptr, &showFilePanel);
			ImGui::MenuItem("Property Panel", nullptr, &showPropertyPanel);
			ImGui::Separator();
			ImGui::MenuItem("Coordinate Axes", nullptr, &showCoordinateAxes);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	for (auto& widget : widgets) {
		widget->renderImGui();
	}

	ImGui::Render();

	// NOTICE: all ImGui Rendering will be processed in
	// Vulkan_Support::VulkanModule::drawFrame
}

/**
 * @brief processing input and data only without read rendering
 */
void Application::processImGuiFrame() {
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& widget : widgets) {
		widget->update();
		// widget->processImGuiData();
	}

	ImGui::EndFrame();
}

void Application::cleanupImGui() {
	// again, before clean up ImGui, make sure vulkan devices are idle
	if (renderWidget) {
		renderWidget->getVulkanModule()->waitDeviceIdle();
	}

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (imguiDescriptorPool != VK_NULL_HANDLE && renderWidget) {
		vkDestroyDescriptorPool(renderWidget->getVulkanModule()->getDevice(),
								imguiDescriptorPool, nullptr);
		imguiDescriptorPool = VK_NULL_HANDLE;
	}
}

VkCommandBuffer Application::beginSingleTimeCommands(
  VulkanModule* vulkanModule) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = vulkanModule->getCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(vulkanModule->getDevice(), &allocInfo,
							 &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

void Application::endSingleTimeCommands(VulkanModule* vulkanModule,
										VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(vulkanModule->getGraphicsQueue(), 1, &submitInfo,
				  VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkanModule->getGraphicsQueue());

	vkFreeCommandBuffers(vulkanModule->getDevice(),
						 vulkanModule->getCommandPool(), 1, &commandBuffer);
}

void Application::renderImGuiSoftware() {
	ImDrawData* drawData = ImGui::GetDrawData();

	if (drawData && drawData->Valid) {
		static int frameCount = 0;
		frameCount++;

		if (frameCount % 30 == 0) {
			char title[256];
			snprintf(title, sizeof(title),
					 "VulkanRenderer - ImGui: %d panels, %d vertices",
					 drawData->CmdListsCount, drawData->TotalVtxCount);
			glfwSetWindowTitle(window, title);
		}

		ImGuiIO& io = ImGui::GetIO();
		if (frameCount % 120 == 0) {
			printf("ImGui Status - Mouse: (%.1f, %.1f), FPS: %.1f\n",
				   io.MousePos.x, io.MousePos.y, io.Framerate);

			printf("Active ImGui Windows: ");
			for (int i = 0; i < drawData->CmdListsCount; i++) {
				printf("Panel%d ", i + 1);
			}
			printf("\n");
		}
	}
}
