#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

enum class Material
{
    BASIC,
    TERRAIN,

    // ensure transparent materials are last
    WATER,
};

class Model
{
public:
    Mesh mesh;
    Material material;

    Model(Mesh, Material);
    Model(Mesh, Material, glm::vec3);

    void translate(glm::vec3);
    void scale(glm::vec3);
    glm::mat4 mvp(const glm::mat4&);

private:
    // add properties, like culling face enable, wireframe rendering etc
    glm::vec3 model_translate, model_scale;
};