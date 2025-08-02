#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "common.h"

#include "UniformData.h"

class Loader {
public:
    Loader();
    void loadModel(const std::string &modelpath);

    void loadSPV(const std::string &spvpath, SPVStage stage);
    std::vector<char> readFile(const std::string &filepath);

    void loadTexture(const std::string &texturepath, TextureType type);
    bool hasTexture(TextureType type);

    void loadMaterial(const std::string &materialpath, MaterialType type);

    const std::vector<Vertex>& getVertices() const;
    const std::vector<uint32_t>& getIndices() const;
    const std::vector<char>& getSPV(SPVStage stage) const;
    const TextureData& getTexture(TextureType type) const;
    const MaterialData& getMaterial(MaterialType type) const;
    
private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<SPVStage, std::vector<char>> spvMap;
    std::unordered_map<TextureType, TextureData> textureMap;
    std::unordered_map<MaterialType, MaterialData> MaterialMap;

    // TODO: extensions
    // std::unordered_map<std::string, BufferData> bufferMap;
    // std::unordered_map<std::string, MeshData> meshMap;
    // std::unordered_map<std::string, AnimationData> animationMap;
};