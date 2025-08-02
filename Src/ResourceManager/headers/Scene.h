#pragma once

#include <memory>
#include <iostream>
#include <ratio>
#include <exception>

#include "Lighting.h"
#include "Loader.h"

class Scene {
   public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	// Factory methods for creating smart pointer instances
	static std::unique_ptr<Scene> create();
	static std::shared_ptr<Scene> createShared();
	// read dirs
	static std::unique_ptr<Scene> createWithAsset(const std::string& assetPath);
	static std::shared_ptr<Scene> createSharedWithAsset(
	  const std::string& assetPath);
	// add
	bool addObject(std::unique_ptr<Loader> object);

	// search
	size_t getObjectCount() const;
	size_t getLightCount() const;
	bool noObjects() const;
	bool noLights() const;

	// getters
	const std::vector<std::unique_ptr<Loader>>& getObjects() const;
	const std::vector<std::unique_ptr<Lighting>>& getLightings() const;

	// manage
	void clear();

   private:
	std::vector<std::unique_ptr<Loader>> objects;
	std::vector<std::unique_ptr<Lighting>> lights;
};