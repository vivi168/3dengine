#pragma once

#include <GL/gl3w.h>
#include <SDL.h>
#include <string>
#include <unordered_map>

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

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

    void create_window(const char*, const int, const int);

    void cache_mesh(Mesh&);
    void cache_texture(Texture&);
    void draw_mesh(Mesh&, Shader&);
};