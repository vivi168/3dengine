#include "Model.h"

#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(Mesh m, Material mat)
    : mesh(m)
    , material(mat)
    , model_translate({ 0.0f, 0.0f, 0.0f })
    , model_scale({ 1.0f, 1.0f, 1.0f })
{

}

Model::Model(Mesh m, Material mat, glm::vec3 t)
    : mesh(m)
    , material(mat)
    , model_translate(t)
    , model_scale({ 1.0f, 1.0f, 1.0f })
{

}

void Model::translate(glm::vec3 t)
{
    model_translate = t;
}

void Model::scale(glm::vec3 s)
{
    model_scale = s;
}

glm::mat4 Model::mvp(const glm::mat4& pv)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, model_translate);
    model = glm::scale(model, model_scale);
    
    return pv * model;
}