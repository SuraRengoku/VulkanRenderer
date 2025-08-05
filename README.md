# VulkanRenderer
#### this is a renderer based on Vulkan API; We may support OpenGL and DirectX later.

## Preview
<img width="912" height="740" alt="revision" src="https://github.com/user-attachments/assets/78bbeeb5-116a-4b01-9731-05746817519b" />

## Installation
### MacOS/Linux
on MacOS, we use default C++ compiler clang, which is included in the Xcode-tools

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
download [glfw](https://www.glfw.org) and extract it. Go into the glfw directory
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make install
# glfw will be installed to C:/Program Files (x86)/GLFW
```

#### GLM
download [glm](https://github.com/g-truc/glm/releases/tag/1.0.1) source code and extract it. Go into the glm directory

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make install
# glm will be installed to C:/Program Files (x86)/glm
```

**Hint:** if you don't want to burden your C disk, you can use *CMAKE_INSTALL_PREFIX* to relocate the installation 

Then you have to add system variables GLFW_HOME and GLM_HOME
```
Key             Value
GLFW_HOME       C:\Program Files (x86)\GLFW
GLM_HOME        C:\Program Files (x86)\glm
```

### Vulkan SDK
download [Vulkan](https://vulkan.lunarg.com) corresponds to your system.

On MacOS, the installation of Vulkan is totally automatic, you just have to make sure the VKcube app can run as usual.

When it comes to the case of Windows, things are different. Although Vulkan will create some system variables by itself when installing, we still have to modify them.

In your system variables, you need to find VK_SDK_PATH and replace it with VULKAN_SDK without changing the value.

### Vulkan Memory Allocator (Optional)

download [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) and extract it to wherever you want.

On MacOS/Linux, you have to add VMA to .zshrc as we have done for GLFW and GLM

```bash
nano ~/.zshrc
export VMA_HOME="/Users/xxxxx/xxxxx/VulkanMemoryAllocator-3.2.1"
# save .zshrc
source ~/.zshrc
```

On Windows, you need to set a system variable VMA_HOME

```
Key             Value
VMA_HOME        X:\XXX\VulkanMemoryAllocator-xxx 
```

### VS Code
all IDE/Editor supporting CMakeList.txt works. You can use Clion or Visual Studio, but VS Code is much more easy and light-weight.

**Enable Extensions:**  
[CMake](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools),  
[clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd),  
[Shader languages support](https://marketplace.visualstudio.com/items?itemName=slevesque.shader),  
[vscode-3d-preview](https://marketplace.visualstudio.com/items?itemName=bierner.3d-preview)

select a C++ compiler(active kit) in VS Code, click **build** button next to it and **run**


