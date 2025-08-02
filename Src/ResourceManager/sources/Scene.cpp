#include "Scene.h"

Scene::Scene() {}

std::unique_ptr<Scene> Scene::create() { return std::make_unique<Scene>(); }

std::shared_ptr<Scene> Scene::createShared() { return std::shared_ptr<Scene>(); }

std::unique_ptr<Scene> Scene::createWithAsset(const std::string& assetPath) {
	auto scene = std::make_unique<Scene>();
	try {

		return scene;
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}

std::shared_ptr<Scene> Scene::createSharedWithAsset(const std::string &assetPath) {
	auto scene = std::make_shared<Scene>();
	try {
		return scene;
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}

bool Scene::addObject(std::unique_ptr<Loader> object) {
	if (!Loader::checkValid(object)) {
		std::cerr << "ResourceManager::Scene::addObject object not valid" << std::endl;
		return false; 
	}
	objects.emplace_back(std::move(object));
	return true; 
}

size_t Scene::getObjectCount() const { return objects.size(); }
size_t Scene::getLightCount() const { return lights.size(); }

bool Scene::noObjects() const { return objects.empty(); }
bool Scene::noLights() const { return lights.empty(); }

const std::vector<std::unique_ptr<Loader>>& Scene::getObjects() const {
	return objects;
}

const std::vector<std::unique_ptr<Lighting>>& Scene::getLightings() const {
	return lights;
}

void Scene::clear() { 
	objects.clear(); 
	lights.clear();
}
