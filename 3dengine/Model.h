#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
public:
    Mesh mesh;
    Shader* shader; // TODO find way to remove shader from here.

    Model(Mesh, Shader*);
    Model(Mesh, Shader*, glm::vec3);
    void upload_mvp(const glm::mat4&);
    void translate(glm::vec3);
    void scale(glm::vec3);
private:
    glm::vec3 model_translate, model_scale;

    glm::mat4 mvp(const glm::mat4&);
};