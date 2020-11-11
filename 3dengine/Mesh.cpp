#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <algorithm>
#include <unordered_map>

unsigned int Mesh::next_id = 0;

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    const int GOLDEN_RATIO = 0x9e3779b9;
    std::hash<T> h;
    seed ^= h(v) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
}

namespace std {
    template<> struct hash<Vertex> {
        std::size_t operator()(Vertex const& vertex) const {
            std::size_t res = 0;
            hash_combine(res, vertex.position);
            hash_combine(res, vertex.normal);
            hash_combine(res, vertex.texture_uv);

            return res;
        }
    };
}

Mesh::Mesh()
    : id(next_id++)
{ }

Mesh::Mesh(const std::string filename, const std::string basedir)
    : id(next_id++)
{
    bool loaded = load_obj(filename, basedir);

     if (!loaded)
         return;
}

Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices)
    : id(next_id++)
    , m_vertices(vertices)
    , m_indices(indices)
{
    // TODO called from height map

    Shape shape;

    shape.name = "terrain";
    shape.indices_start = 0;
    shape.indices_count = static_cast<unsigned int>(m_indices.size());

    std::vector<std::string> textures = {
        "assets/blendmap.png",
        "assets/grass.png",
        "assets/paved.png",
        "assets/mud.png",
        "assets/grassFlowers.png"
    };

    std::vector<std::string> uniforms = {
        "blendmap",
        "base_texture",
        "r_texture",
        "g_texture",
        "b_texture"
    };

    for (int i = 0; i < 5; i++) {
        Texture t = { uniforms[i], textures[i] };
        shape.textures.push_back(t);
    }

    m_shapes.push_back(shape);
}

bool Mesh::load_obj(const std::string filename, const std::string basedir)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), basedir.c_str(), true);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!loaded) {
        std::cerr << "Failed to load / parse.obj" << std::endl;
        return false;
    }
    std::cout << "LOADED OBJ " << filename << std::endl;

    std::unordered_map<Vertex, unsigned int> unique_vertices{};

    for (const auto& shape : shapes) {
        Shape s;
        s.indices_start = static_cast<unsigned int>(m_indices.size());

        for (const auto& index : shape.mesh.indices) {
            Vertex vertex;

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertex.texture_uv = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            if (unique_vertices.count(vertex) == 0) {
                unique_vertices[vertex] = static_cast<unsigned int>(m_vertices.size());
                m_vertices.push_back(vertex);
            }

            m_indices.push_back(unique_vertices[vertex]);
        }

        s.name = shape.name;
        s.indices_count = static_cast<unsigned int>(m_indices.size()) - s.indices_start;
        
        Texture t = {
            "texture_sampler",
            basedir + materials[shape.mesh.material_ids[0]].diffuse_texname
        };
        s.textures.push_back(t);

        m_shapes.push_back(s);
        std::cout << "CREATED SHAPE " << s.name << std::endl;
    }

    std::cout << m_vertices.size() << std::endl;
    std::cout << m_indices.size() << std::endl;

    return true;
}
