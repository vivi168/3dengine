#pragma once

#include <GL/gl3w.h>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"

struct TextureCache
{
    GLuint id;
    std::string name;

    void init(Texture&);
    void destroy();
};

struct MeshCache
{
    GLuint vertex_array_obj, vertex_buffer_obj, element_buffer_obj;

    enum {
        POSITION_VB,
        NORMAL_VB,
        TEXTUV_VB,
    };

    void init(Mesh&);
    void destroy();
};

class Renderer
{
public:
    Renderer();
    void init();
    void render(Scene&, Camera&);
    void cleanup();

private:
    SDL_Window* window;
    SDL_GLContext context;

    std::unordered_map<unsigned int, MeshCache> mesh_cache;
    std::unordered_map<std::string, TextureCache> texture_cache;
    std::unordered_map<Material, Shader> shaders;

    void create_window(const char*, const int, const int);

    void cache_mesh(Mesh&);
    void cache_texture(Texture&);
    Shader shader_for(Material);

    void draw_mesh(Mesh&, Shader&);
};
