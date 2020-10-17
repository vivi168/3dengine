#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Mesh::init(std::vector<Vertex> v, std::vector<GLuint> i, std::vector<Texture> t)
{
    vertices = v;
    indices = i;
    textures = t;

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

        glUniform1i(glGetUniformLocation(shader.id(), "texture_sampler"), i);
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

Texture load_texture(const std::string path)
{
    GLuint texture_id;
    Texture texture;
    int width, height, channels, mode;
    unsigned char* img_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    mode = channels == 4 ? GL_RGBA : GL_RGB;

    glGenTextures(1, &texture_id);

    if (img_data) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, img_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Error while loading image\n";
    }

    stbi_image_free(img_data);

    texture.id = texture_id;
    texture.path = path;

    return texture;
}