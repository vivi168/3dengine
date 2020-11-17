#pragma once

#include <vector>
#include <unordered_map>

#include "Model.h"

class Scene
{
public:
    std::unordered_map<Material, std::vector<Model>> models;
    // TODO: add light, LODs
    Scene()
    {

    }

    void add_model(Model m)
    {
        models[m.material].push_back(m);
    }
private:
};