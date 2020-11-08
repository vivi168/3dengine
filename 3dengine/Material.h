#pragma once

#include <string>
#include <vector>

class Texture
{
public:
    const unsigned int id;
    std::string path;

    Texture();
    Texture(const std::string);
private:
    static unsigned int next_id;
};

struct Material
{
    std::string name;
    Texture texture;
};