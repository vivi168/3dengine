#include "Water.h"

Mesh water_quad(int width, int height)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i < (width * height); i++) {
        Vertex vertex;

        int x, y, z;
        x = (i % width);
        z = (i / width);

        vertex.position = {
            x,
            0,
            z
        };

        // TODO
        vertex.normal = {
            0, 0 ,0
        };

        // TODO
        vertex.texture_uv = {
            (float)x / (width - 1),
            (float)z / (height - 1)
        };

        vertices.push_back(vertex);

        // tweak the indices order for face culling
        if (x < width - 1) {
            if (z > 0) {
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + 1 - width);
            }

            if (z < height - 1) {
                indices.push_back(i + width);
                indices.push_back(i + 1);
                indices.push_back(i);
            }
        }
    }

    return Mesh(vertices, indices, std::vector<Texture>());
}