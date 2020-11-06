#pragma once

#include <GL/gl3w.h>
#include <string>
#include <unordered_map>

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

private:
    std::unordered_map<unsigned int, MeshCache> mesh_cache;
    std::unordered_map<unsigned int, TextureCache> texture_cache;
};