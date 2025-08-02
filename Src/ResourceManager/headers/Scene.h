#pragma once

#include "Loader.h"

class Scene {
public:
    Scene();

    void addObject(const Loader object);
    
private:
    std::vector<Loader> objects;
    std::vector<LightData> lights;
};