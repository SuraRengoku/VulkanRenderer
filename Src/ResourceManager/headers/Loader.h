#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "UniformData.h"
#include "common.h"

#include "stb_image.h"
#include "tiny_obj_loader.h"

class Loader {
   public:
	Loader();
	Loader(const Loader&) = delete;
	Loader& operator=(const Loader&) = delete;

	// Factory methods for creating smart pointer instances
	static std::unique_ptr<Loader> create();
	static std::shared_ptr<Loader> createShared();
	// read dirs
	static std::unique_ptr<Loader> createWithAsset(
	  const std::string& assetPath);
	static std::shared_ptr<Loader> createSharedWithAsset(
	  const std::string& assetPath);

	// Loading methods
	void loadModel(const std::string& modelpath);
	void loadSPV(const std::string& spvpath, SPVStage stage);
	std::vector<char> readFile(const std::string& filepath);
	void loadTexture(const std::string& texturepath, TextureType type);
	bool hasTexture(TextureType type);
	void loadMaterial(const std::string& materialpath, MaterialType type);
	void loadAsset(const std::string assetpath);

	// Getters
	const std::vector<Vertex>& getVertices() const;
	const std::vector<uint32_t>& getIndices() const;
	const std::vector<char>& getSPV(SPVStage stage) const;
	const TextureData& getTexture(TextureType type) const;
	const MaterialData& getMaterial(MaterialType type) const;

	// Validation interface
	static bool checkValid(const Loader& object);
	static bool checkValid(const std::unique_ptr<Loader>& object);
	static bool checkValid(const std::shared_ptr<Loader>& object);
	bool isValid() const;

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