# VulkanRenderer
#### this is a renderer based on Vulkan API; We may support OpenGL and DirectX later.

## Preview
<img width="868" height="696" alt="vulkanpreview" src="https://github.com/user-attachments/assets/d8e68806-89ac-4c8d-88e2-bd89e2f1b66b" />

## Installation
### MacOS/Linux
on MaxOS, we use default C++ compiler clang, which is included in the Xcode-tools

#### GLFW

```bash
brew install glfw
```

#### GLM

```bash 
brew install glm
```

#### create PATH of GLFW and GLM

``` bash
nano ~/.zshrc #you may use other shells
# add the following lines to .zshrc, you may change the relative path
export GLFW_HOME="/opt/homebrew/Cellar/glfw/3.4"
export GLM_HOME="/opt/homebrew/Cellar/glm/1.0.1"
# save .zshrc
source ~/.zshrc
```

### Windows
on Windows, we use GCC; You can try MSVC but we don't promise it will work

#### GLFW


#### GLM

### Vulkan Memory Allocator (Optional)

download Vulkan Memory Allocator and extract it to wherever you want.

On MacOS, you have to add VMA to .zshrc as we have done for GLFW and GLM

```bash
nano ~/.zshrc
export VMA_HOME="/Users/xxxxx/xxxxx/VulkanMemoryAllocator-3.2.1"
# save .zshrc
source ~/.zshrc
```

### VS Code
all IDE/Editor supporting CMakeList.txt works. You can use Clion or Visual Studio, but VS Code is easy to use.

<span style="color:cyan">
enable Extensions: Cmake, clangd, Shader languages support, vscode-3d-preview
</span>
<br>
<br>
build and run


