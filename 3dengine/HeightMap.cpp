#include "HeightMap.h"
#include <stb_image.h>

#include <iostream>

HeightMap::HeightMap(const std::string filepath)
{
    int width, height;

    unsigned char* img_data = stbi_load(filepath.c_str(), &width, &height, nullptr, 1);

    for (int i = 0; i < (width * height); i ++) {
        pixels.push_back(img_data[i]);
    }

    stbi_image_free(img_data);

    gen_vertices(width);
}

void HeightMap::gen_vertices(int width)
{
    std::cout << "Generate mesh " << width << std::endl;
    int height = pixels.size() / width;
    for (int i = 0; i < pixels.size(); i++) {
        Vertex vertex;

        int x, y;
        x = (i % width);
        y = (i / width);
        
        vertex.position = {
            x,
            pixels[i] / 64,
            y
        };

        // TODO
        vertex.normal = {
            0, 0 ,0
        };

        // TODO
        vertex.texture_uv = {
            0, 0
        };

        vertices.push_back(vertex);

        if (x < width - 1) {
            if (y > 0) {
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + 1 - width);
            }
            
            if (y < height - 1) {
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + width);
            }
        }
    }

    std::cout << vertices.size() << std::endl;
    std::cout << indices.size() << std::endl;
}

Mesh HeightMap::mesh()
{
    return Mesh(vertices, indices);
}