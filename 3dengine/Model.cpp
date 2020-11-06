#include "Model.h"
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>

Model::Model(Mesh m, Shader* s)
    : mesh(m)
    , shader(s)
    , model_translate({ 0.0f, 0.0f, 0.0f })
    , model_scale({ 1.0f, 1.0f, 1.0f })
{

}

Model::Model(Mesh m, Shader* s, glm::vec3 t)
    : mesh(m)
    , shader(s)
    , model_translate(t)
    , model_scale({ 1.0f, 1.0f, 1.0f })
{

}

void Model::draw(const glm::mat4& pv)
{
    shader->use();

    glm::mat4 mat = mvp(pv);

    GLuint mvp_loc = glGetUniformLocation(shader->id(), "mvp");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mat));

    mesh.draw(*shader);
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

void Model::cleanup()
{
    mesh.cleanup();
}