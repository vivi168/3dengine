#pragma once

#include "Mesh.h"

#include <vector>

class Water
{
public:
    Water(int, int);
    Mesh mesh();
private:
    void gen_vertices();

    int width, height; // width/height of water quad
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};
