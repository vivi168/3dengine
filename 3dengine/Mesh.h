#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_uv;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texture_uv == other.texture_uv;
    }
};

struct Texture {
    GLuint id;
    std::string name;
};

struct Shape {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    std::string name;
    int material_id;
    GLuint vertex_array_obj, vertex_buffer_obj, element_buffer_obj;
};

class Mesh
{
private:
    GLuint vertex_array_obj, vertex_buffer_obj, element_buffer_obj;

    enum {
        POSITION_VB,
        NORMAL_VB,
        TEXTUV_VB,
    };

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    std::vector<Shape> m_shapes;

    void init();
    void init_shapes();
    GLuint load_texture(const std::string);
    bool load_obj(const std::string, const std::string);

public:
    Mesh();
    Mesh(const std::string, const std::string);
    Mesh(const std::vector<Vertex>, const std::vector<GLuint>);
    void draw(const Shader&);
    void draw_shapes(const Shader&);
    void cleanup();
};