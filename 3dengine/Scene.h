#pragma once

#include <vector>
#include "Model.h"

class Scene
{
public:
    std::vector<Model> models;

    Scene()
    {

    }

    void add_model(Model m)
    {
        models.push_back(m);
    }
private:
    
};