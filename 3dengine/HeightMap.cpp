#include "HeightMap.h"
#include <stb_image.h>

#include <iostream>

HeightMap::HeightMap(const std::string filepath)
{
    int width, height;

    unsigned char* img_data = stbi_load(filepath.c_str(), &width, &height, nullptr, 1);

    min = 0; max = 0;

    for (int i = 0; i < (width * height); i ++) {
        pixels.push_back(img_data[i]);

        if (img_data[i] > max) max = img_data[i];
        if (img_data[i] < min) min = img_data[i];
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

        int x, y, z;
        x = (i % width);
        z = (i / width);
        
        y = pixels[i];

        vertex.position = {
            x,
            (float)y / 255.0f * 50.0f,
            -z
        };

        // TODO
        vertex.normal = {
            0, 0 ,0
        };

        // TODO
        vertex.texture_uv = {
            x,
            -z
        };

        vertices.push_back(vertex);

        // tweak the indices order for face culling
        if (x < width - 1) {
            if (z > 0) {
                indices.push_back(i + 1 - width);
                indices.push_back(i + 1);
                indices.push_back(i);
            }
            
            if (z < height - 1) {
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
    // TODO specify texture(s) path here ?
    return Mesh(vertices, indices);
}