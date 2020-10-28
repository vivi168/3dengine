#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
    Model(Mesh);
    Model(Mesh, glm::vec3);
    void draw(Shader& shader, glm::mat4);
    void translate(glm::vec3);
    void scale(glm::vec3);
    void cleanup();
private:
    Mesh mesh;
    glm::vec3 model_translate, model_scale;

    glm::mat4 mvp(glm::mat4);
};