#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Unable to init SDL\n";
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    create_window("3D Engine", WINDOW_WIDTH, WINDOW_HEIGHT);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // TODO : add way to add these 3 per model as option?
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void Renderer::create_window(const char* title, const int width, const int height)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window == NULL) {
        std::cerr << "Error while creating SDL_Window\n";
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    context = SDL_GL_CreateContext(window);

    if (gl3wInit()) {
        std::cerr << "failed to init GL3W" << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Renderer::render(Scene& scene, Camera &camera)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: add way to set wireframe rendering option per model
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 view = camera.look_at();
    glm::mat4 projection = glm::perspective(camera.zoom(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 pv = projection * view;

    for (auto model : scene.models) {
        Material mat = model.first;
        Shader shader = shader_for(mat);

        shader.use();

        for (auto m : model.second) {
            glm::mat4 mat = m.mvp(pv);
            GLuint mvp_loc = glGetUniformLocation(shader.id(), "mvp");
            glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mat));

            cache_mesh(m.mesh);
            for (auto shape : m.mesh.m_shapes) {
                for (auto texture : shape.textures)
                    cache_texture(texture);
            }
            draw_mesh(m.mesh, shader);
        }
    }

    SDL_GL_SwapWindow(window);
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
    if (texture_cache.find(texture.path) != texture_cache.end()) {
        return;
    }

    TextureCache tc;
    tc.init(texture);

    texture_cache[texture.path] = tc;
}

Shader Renderer::shader_for(Material mat)
{
    if (shaders.find(mat) != shaders.end()) {
        return shaders[mat];
    }

    if (mat == Material::TERRAIN) {
        shaders[mat].load("terrain", "shaders/");
    }
    else {
        shaders[mat].load("shader", "shaders/");
    }

    return shaders[mat];
}

void Renderer::draw_mesh(Mesh& mesh, Shader& shader)
{
    glBindVertexArray(mesh_cache[mesh.id].vertex_array_obj);

    for (auto shape : mesh.m_shapes) {
        for (int i = 0; i < shape.textures.size(); i++) {
            const std::string key = shape.textures[i].path;
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(glGetUniformLocation(shader.id(), shape.textures[i].name.c_str()), i);
            glBindTexture(GL_TEXTURE_2D, texture_cache[key].id);
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

    for (auto s : shaders) {
        s.second.unlink();
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();
}