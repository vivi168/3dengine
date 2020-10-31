#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <algorithm>
#include <unordered_map>

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

Mesh::Mesh() { }

Mesh::Mesh(const std::string filename, const std::string basedir)
{
    bool loaded = load_obj(filename, basedir);

     if (!loaded)
         return;

     init();

}

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<GLuint> i)
{
    // TODO called from height map
    vertices = v;
    indices = i;

    textures.push_back({ load_texture("assets/blendmap.png"), "blendmap" });
    textures.push_back({ load_texture("assets/grass.png"), "base_texture" });
    textures.push_back({ load_texture("assets/paved.png"), "r_texture" });
    textures.push_back({ load_texture("assets/mud.png"), "g_texture" });
    textures.push_back({ load_texture("assets/grassFlowers.png"), "b_texture" });

    init();
}

void Mesh::init()
{
    glGenVertexArrays(1, &vertex_array_obj);
    glGenBuffers(1, &vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);

    glBindVertexArray(vertex_array_obj);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // vertex position
    glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(POSITION_VB);

    // vertex normals
    glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(NORMAL_VB);

    // vertex texture coordinates
    glVertexAttribPointer(TEXTUV_VB, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_uv));
    glEnableVertexAttribArray(TEXTUV_VB);

    glBindVertexArray(0);
}

void Mesh::draw(const Shader &shader)
{
    for (auto i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        glUniform1i(glGetUniformLocation(shader.id(), textures[i].name.c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(vertex_array_obj);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup()
{
    glDeleteVertexArrays(1, &vertex_array_obj);
    glDeleteBuffers(1, &vertex_buffer_obj);
    glDeleteBuffers(1, &element_buffer_obj);

    for (auto t : textures) {
        glDeleteTextures(1, &t.id);
    }
}

GLuint Mesh::load_texture(const std::string path)
{
    // todo load texture once
    GLuint texture_id;
    int width, height, channels, mode;
    unsigned char* img_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    mode = channels == 4 ? GL_RGBA : GL_RGB;

    std::cout << path << channels << mode << std::endl;

    glGenTextures(1, &texture_id);

    if (img_data) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, img_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "Error while loading image\n";
    }

    stbi_image_free(img_data);

    return texture_id;
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

    std::unordered_map<Vertex, uint32_t> unique_vertices{};

    for (const auto& shape : shapes) {
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
                unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(unique_vertices[vertex]);
        }
    }

    std::cout << vertices.size() << std::endl;
    std::cout << indices.size() << std::endl;

    // TODO: multiple textures ?
    GLuint tid = load_texture(basedir + materials[0].diffuse_texname);
    Texture t = { tid, "texture_sampler" };
    textures.push_back(t);

    return true;
}