#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_uv;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texture_uv == other.texture_uv;
    }
};

struct Texture
{
    std::string name, path;
};

struct Shape
{
    std::string name;
    unsigned int indices_count, indices_start;
    std::vector<Texture> textures;
};

class Mesh
{
public:
    const unsigned int id;

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Shape> m_shapes;

    Mesh();
    Mesh(const std::string, const std::string);
    Mesh(const std::vector<Vertex>, const std::vector<GLuint>);

private:
    static unsigned int next_id;

    bool load_obj(const std::string, const std::string);
};
