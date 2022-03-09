#pragma once

#include <vector>
#include <map>

#include "Model.h"

class Scene
{
public:
    // TODO maybe discard model class, and add models information to a scene node.
    // scene nodes contains a mesh ?
    // delete model, rename mesh -> model, rename shape -> mesh ?


    // TODO have a "Scene" model, which is a tree, containing nodes (model, billboards, etc) to render.
    // render in correct order.
    std::map<Material, std::vector<Model>> models;
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