#include "Loader.h"
#include <stdexcept>
#include "UniformData.h"
#include "common.h"

Loader::Loader() {

}

void Loader::loadModel(const std::string &modelpath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelpath.c_str())) {
        throw std::runtime_error("ResourceManager::Loader::loadModel " + warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for(const auto &shape : shapes) {
        for(const auto &index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],                     
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {1.0f, 1.0f, 1.0f};
            
            if(uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.emplace_back(vertex);
            }
            indices.emplace_back(uniqueVertices[vertex]); // index starts from 0
        }
    }
}

void Loader::loadSPV(const std::string &spvpath, SPVStage stage) {
    auto data = readFile(spvpath);
    this -> spvMap[stage] = std::move(data);
}

std::vector<char> Loader::readFile(const std::string &filepath) {
    // ate -> read from the end in order to know the size of file
    // binary -> read as binary format
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    
    if(!file.is_open()) {
        throw std::runtime_error("ResourceManager::Loader::readSPVFile failed to open file" + filepath);
    } 

    size_t fileSize = (size_t)file.tellg(); // return byte counts
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}

void Loader::loadTexture(const std::string &texturepath, TextureType type) {
    int texWidth, texHeight, texChannels;
    stbi_uc* data = stbi_load(texturepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if(!data) {
        throw std::runtime_error("ResourceManager::Loader::loadTexture failed to load texture: " + texturepath);
    }

    TextureData tex;
    tex.width = texWidth;
    tex.height = texHeight;
    tex.channels = texChannels;
    tex.pixels.assign(data, data + texWidth * texHeight * texChannels);
    stbi_image_free(data);

    textureMap[type] = std::move(tex);
}

void Loader::loadMaterial(const std::string &materialpath, MaterialType type) {
    // TODO
}

const std::vector<Vertex>& Loader::getVertices() const {
    if(this -> vertices.empty()) {
        throw std::runtime_error("ResourceManager::Loader::getVertices no vertex data!");
    }
    return this -> vertices;
}

const std::vector<uint32_t>& Loader::getIndices() const {
    if(this -> indices.empty()) {
        throw std::runtime_error("ResourceManager::Loader::getIndices no index data!");
    }
    return this -> indices;
}

const std::vector<char>& Loader::getSPV(SPVStage stage) const {
    auto it = spvMap.find(stage);
    if(it == spvMap.end()) {
        throw std::runtime_error("ResourceManager::Loader::getSPV SPV not loaded for stage " + EnumToString(stage));
    }
    return it -> second;
}

const TextureData& Loader::getTexture(TextureType type) const {
    auto it = textureMap.find(type);
    if(it == textureMap.end()) {
        throw std::runtime_error("ResourceManager::Loader::getTexture " + EnumToString(type) + " texture not loaded");
    }
    return it -> second;
}

const MaterialData& Loader::getMaterial(MaterialType type) const {
    auto it = MaterialMap.find(type);
    if(it == MaterialMap.end()) {
        throw std::runtime_error("ResourceManager::Loader::getMaterial " + EnumToString(type) + " material not loaded");
    }
    return it -> second;
};