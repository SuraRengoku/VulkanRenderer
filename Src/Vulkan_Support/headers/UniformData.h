# pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vulkan/vulkan.h>

#include "common.h"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;  // index
        bindingDescription.stride = sizeof(Vertex);
        /*
        * VK_VERTEX_INPUT_RATE_VERTEX: move forward after each vertex
        * VK_VERTEX_INPUT_RATE_INSTANCE: move forward after each instance
        */
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const {
        return pos == other.pos && color == other.color &&
            texCoord == other.texCoord;
    }
};

// even though specific Vulkan implementation may support window system, these
// implementations in all platform may not support the same feature
struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;
    bool isComplete() const { return graphicsFamily >= 0 && presentFamily >= 0; }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace std {
    template <> struct hash<Vertex> {
        size_t operator()(Vertex const &vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                    (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}  // namespace std

struct TextureData {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> pixels;
};

struct MaterialData {
    MaterialType type = MaterialType::Lambert; 

    glm::vec3 baseColor = glm::vec3(1.0f);     
    float metallic = 0.0f;                     
    float roughness = 1.0f;                    
    float opacity = 1.0f;                      
    float emissiveStrength = 0.0f;             

    int albedoTexture = -1;
    int normalTexture = -1;
    int metallicTexture = -1;
    int roughnessTexture = -1;
    int aoTexture = -1;
    int emissiveTexture = -1;
};

// TODO 
struct BufferData {};
// TODO
struct MeshData {};
// TODO
struct AnimationData {};

struct LightData {
    LightType type = LightType::Point;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    // for attenuation
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    float innerCutoff = glm::cos(glm::radians(12.5f));
    float outerCutoff = glm::cos(glm::radians(17.5f));

    bool castShadow = false;
};