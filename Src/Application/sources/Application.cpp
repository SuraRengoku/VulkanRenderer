#include "Application.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "renderWidget.h"
#include "FileWidget.h"
#include "PropertyWidget.h"

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
    // 创建共享场景
    scene = std::make_shared<Scene>();
    
    // 创建渲染Widget（仅负责Vulkan 3D渲染）
    auto renderWidgetPtr = std::make_unique<RenderWidget>(window, 0, 0, WIDTH, HEIGHT, true, true, WidgetStatus::ACTIVATED);
    renderWidget = renderWidgetPtr.get();
    widgets.push_back(std::move(renderWidgetPtr));
    
    // 创建文件Widget（纯ImGui界面，无Vulkan渲染）
    auto fileWidgetPtr = std::make_unique<FileWidget>(window, 0, 0, 300, HEIGHT, true, true, WidgetStatus::ACTIVATED);
    fileWidget = fileWidgetPtr.get();
    widgets.push_back(std::move(fileWidgetPtr));
    
    // 创建属性Widget（纯ImGui界面，无Vulkan渲染）
    auto propertyWidgetPtr = std::make_unique<PropertyWidget>(window, WIDTH-300, 0, 300, HEIGHT, true, true, WidgetStatus::ACTIVATED);
    propertyWidget = propertyWidgetPtr.get();
    widgets.push_back(std::move(propertyWidgetPtr));
    
    // 确保Vulkan完全初始化后再初始化ImGui
    renderWidget->getVulkanModule()->waitDeviceIdle();
    
    // 初始化ImGui（使用Vulkan后端）
    initImGui();
    
    // 主渲染循环
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Vulkan渲染：仅3D场景内容
        renderWidget->display();
        
        // ImGui渲染：所有UI界面 + 辅助元素
        renderImGui();
        
        // 更新所有Widget状态
        for(auto& widget : widgets) {
            widget->update();
        }
        
        glfwSwapBuffers(window);
    }
    
    // 确保Vulkan设备完全空闲后再清理ImGui
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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // 初始化GLFW输入处理
    ImGui_ImplGlfw_InitForVulkan(window, true);
    
    // 设置显示尺寸
    io.DisplaySize = ImVec2((float)WIDTH, (float)HEIGHT);
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    
    // 获取Vulkan对象用于ImGui初始化
    VulkanModule* vulkanModule = renderWidget->getVulkanModule();
    
    // 创建ImGui专用的descriptor pool
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = std::size(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(vulkanModule->getDevice(), &poolInfo, nullptr, &imguiDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create ImGui descriptor pool!");
    }
    
    // 配置ImGui Vulkan初始化信息
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanModule->getInstance();
    init_info.PhysicalDevice = vulkanModule->getPhysicalDevice();
    init_info.Device = vulkanModule->getDevice();
    init_info.QueueFamily = vulkanModule->getGraphicsQueueFamily();
    init_info.Queue = vulkanModule->getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = imguiDescriptorPool;  // 使用专用的descriptor pool
    init_info.RenderPass = vulkanModule->getRenderPass();
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = vulkanModule->getSwapChainImageCount();
    init_info.MSAASamples = vulkanModule->getMSAASamples();  // 使用与主渲染管线相同的MSAA设置
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = nullptr;
    
    // 初始化ImGui Vulkan后端
    ImGui_ImplVulkan_Init(&init_info);
    
    // 创建字体纹理
    ImGui_ImplVulkan_CreateFontsTexture();
}

void Application::renderImGui() {
    // 开始新帧
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 渲染主菜单栏
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Import Model")) {
                // fileWidget->openFileDialog();
            }
            if(ImGui::MenuItem("Export Scene")) {
                // 导出场景
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")) {
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

    // 渲染所有Widget的ImGui界面
    for(auto& widget: widgets) {
        widget->renderImGui();
    }

    // 在RenderWidget区域上覆盖渲染坐标轴指示器（ImGui负责）
    renderCoordinateAxes();
    
    // 渲染性能监控信息
    renderPerformanceOverlay();

    // 完成ImGui渲染
    ImGui::Render();
    
    // 注意：实际的ImGui Vulkan渲染将在VulkanModule的drawFrame中进行
}

void Application::renderCoordinateAxes() {
    // 在渲染区域的左下角显示坐标轴
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowPos = ImVec2(50, io.DisplaySize.y - 100);
    ImVec2 windowSize = ImVec2(80, 80);
    
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowBgAlpha(0.3f);
    
    if(ImGui::Begin("Coordinate Axes", nullptr, 
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 center = ImVec2(windowPos.x + 40, windowPos.y + 40);
        
        // X轴 (红色)
        drawList->AddLine(center, ImVec2(center.x + 30, center.y), IM_COL32(255, 0, 0, 255), 2.0f);
        drawList->AddText(ImVec2(center.x + 32, center.y - 8), IM_COL32(255, 0, 0, 255), "X");
        
        // Y轴 (绿色)
        drawList->AddLine(center, ImVec2(center.x, center.y - 30), IM_COL32(0, 255, 0, 255), 2.0f);
        drawList->AddText(ImVec2(center.x + 2, center.y - 32), IM_COL32(0, 255, 0, 255), "Y");
        
        // Z轴 (蓝色) - 斜向表示深度
        drawList->AddLine(center, ImVec2(center.x - 20, center.y + 20), IM_COL32(0, 0, 255, 255), 2.0f);
        drawList->AddText(ImVec2(center.x - 25, center.y + 22), IM_COL32(0, 0, 255, 255), "Z");
    }
    ImGui::End();
}

void Application::renderPerformanceOverlay() {
    // 在右上角显示FPS和性能信息
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowPos = ImVec2(io.DisplaySize.x - 200, 25);
    
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowBgAlpha(0.3f);
    
    if(ImGui::Begin("Performance", nullptr, 
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | 
        ImGuiWindowFlags_AlwaysAutoResize)) {
        
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
        
        // 可以添加更多性能信息
        if(renderWidget) {
            ImGui::Text("Render Mode: %s", renderWidget->getWireframeMode() ? "Wireframe" : "Solid");
            ImGui::Text("FOV: %.1f°", renderWidget->getFOV());
        }
    }
    ImGui::End();
}

void Application::processImGuiFrame() {
    // 只处理ImGui输入和数据，不进行实际渲染
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 处理所有Widget的ImGui逻辑（仅数据更新，无视觉输出）
    for(auto& widget: widgets) {
        // 这里可以处理Widget的数据更新，但不渲染
        // widget->processImGuiData();
    }

    // 完成帧处理但不渲染
    ImGui::EndFrame();
}

void Application::cleanupImGui() {
    // 确保设备空闲后再清理ImGui
    if (renderWidget) {
        renderWidget->getVulkanModule()->waitDeviceIdle();
    }
    
    // 清理ImGui
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // 清理ImGui专用的descriptor pool
    if (imguiDescriptorPool != VK_NULL_HANDLE && renderWidget) {
        vkDestroyDescriptorPool(renderWidget->getVulkanModule()->getDevice(), imguiDescriptorPool, nullptr);
        imguiDescriptorPool = VK_NULL_HANDLE;
    }
}

VkCommandBuffer Application::beginSingleTimeCommands(VulkanModule* vulkanModule) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vulkanModule->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vulkanModule->getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void Application::endSingleTimeCommands(VulkanModule* vulkanModule, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(vulkanModule->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vulkanModule->getGraphicsQueue());

    vkFreeCommandBuffers(vulkanModule->getDevice(), vulkanModule->getCommandPool(), 1, &commandBuffer);
}

void Application::renderImGuiSoftware() {
    // 简单的ImGui状态显示 - 通过控制台和窗口标题显示
    ImDrawData* drawData = ImGui::GetDrawData();
    
    if (drawData && drawData->Valid) {
        static int frameCount = 0;
        frameCount++;
        
        // 更新窗口标题显示ImGui状态
        if (frameCount % 30 == 0) {
            char title[256];
            sprintf(title, "VulkanRenderer - ImGui: %d panels, %d vertices", 
                    drawData->CmdListsCount, drawData->TotalVtxCount);
            glfwSetWindowTitle(window, title);
        }
        
        // 显示一些基本的ImGui交互状态
        ImGuiIO& io = ImGui::GetIO();
        if (frameCount % 120 == 0) {
            printf("ImGui Status - Mouse: (%.1f, %.1f), FPS: %.1f\n", 
                   io.MousePos.x, io.MousePos.y, io.Framerate);
            
            // 显示当前活跃的窗口
            printf("Active ImGui Windows: ");
            for (int i = 0; i < drawData->CmdListsCount; i++) {
                printf("Panel%d ", i + 1);
            }
            printf("\n");
        }
    }
}