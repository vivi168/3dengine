#include "Water.h"

Water::Water(int w, int h)
    : width(w)
    , height(h)
{
    gen_vertices();
}

void Water::gen_vertices()
{
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
}

Mesh Water::mesh()
{
    std::vector<Texture> textures {
        {
            "texture_sampler",
            "assets/Water_002_DISP.png"
        }
    };

    return Mesh(vertices, indices, textures);
}