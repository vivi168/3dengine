#pragma once

#include "Mesh.h"

#include <vector>
#include <string>

class HeightMap
{
private:
    std::vector<unsigned int> pixels;
    int min, max;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    HeightMap(const std::string);
    void gen_vertices(int);
    Mesh mesh();
};