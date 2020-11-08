#include "Material.h"

#include <iostream>

unsigned int Texture::next_id = 0;

Texture::Texture()
    :id(next_id++)
{

}

Texture::Texture(const std::string p)
    : id(next_id++)
    , path(p)
{

}