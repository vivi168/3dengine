#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Renderer.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

void TextureCache::init(Texture& texture)
{
    int width, height, channels, mode;
    unsigned char* img_data = stbi_load(texture.path.c_str(), &width, &height, &channels, 0);

    mode = channels == 4 ? GL_RGBA : GL_RGB;

    glGenTextures(1, &id);

    if (img_data) {
        glBindTexture(GL_TEXTURE_2D, id);
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
}

void TextureCache::destroy()
{
    glDeleteTextures(1, &id);
}

void MeshCache::init(Mesh& mesh)
{
    glGenVertexArrays(1, &vertex_array_obj);
    glGenBuffers(1, &vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);

    glBindVertexArray(vertex_array_obj);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, mesh.m_vertices.size() * sizeof(Vertex), &mesh.m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.size() * sizeof(GLuint), &mesh.m_indices[0], GL_STATIC_DRAW);

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

void MeshCache::destroy()
{
    glDeleteVertexArrays(1, &vertex_array_obj);
    glDeleteBuffers(1, &vertex_buffer_obj);
    glDeleteBuffers(1, &element_buffer_obj);
}

Renderer::Renderer()
{

}

void Renderer::init()
{
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
}

void Renderer::render(Scene& scene, Camera &camera)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.look_at();
    // TODO find a nice way to pass window width/height
    glm::mat4 projection = glm::perspective(camera.zoom(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 pv = projection * view;

    for (auto m : scene.models) {
        m.upload_mvp(pv);

        cache_mesh(m.mesh);
        for (auto shape : m.mesh.m_shapes) {
            for (auto material : shape.materials)
                cache_texture(material.texture);
        }
        draw_mesh(m.mesh, *m.shader);
    }
}

void Renderer::cache_mesh(Mesh& mesh)
{
    if (mesh_cache.find(mesh.id) != mesh_cache.end()) {
        return;
    }

    MeshCache mc;
    mc.init(mesh);

    mesh_cache[mesh.id] = mc;
}

void Renderer::cache_texture(Texture& texture)
{
    if (texture_cache.find(texture.id) != texture_cache.end()) {
        return;
    }

    TextureCache tc;
    tc.init(texture);

    texture_cache[texture.id] = tc;
}

void Renderer::draw_mesh(Mesh& mesh, Shader& shader)
{
    glBindVertexArray(mesh_cache[mesh.id].vertex_array_obj);

    for (auto shape : mesh.m_shapes) {
        for (auto material : shape.materials) {
            int id = material.texture.id;
            glActiveTexture(GL_TEXTURE0 + id);
            glUniform1i(glGetUniformLocation(shader.id(), material.name.c_str()), id);
            glBindTexture(GL_TEXTURE_2D, texture_cache[id].id);
        }

        glDrawElements(GL_TRIANGLES, shape.indices_count, GL_UNSIGNED_INT, (void*)(sizeof(int) * shape.indices_start));
    }

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::cleanup()
{
    for (auto mc : mesh_cache) {
        mc.second.destroy();
    }

    for (auto tc : texture_cache) {
        tc.second.destroy();
    }
}