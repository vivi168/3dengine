#pragma once

#include <vector>
#include "Model.h"

class Scene
{
public:
    std::vector<Model> models;
    // TODO: add light, LODs
    Scene()
    {

    }

    void add_model(Model m)
    {
        models.push_back(m);
    }
private:
};