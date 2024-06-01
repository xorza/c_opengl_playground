#pragma once

#include "../utils/gl_includes.hpp"
#include "../utils/nocopy.hpp"
#include "../utils/glm_includes.hpp"

struct Texture {
    NOCOPY(Texture)

    uint32_t id = 0;
    uint32_t width = 0;
    uint32_t height = 0;

    Texture() = default;

    ~Texture();

    void bind(uint32_t unit);

    void upload(const void *data);
};

int create_texture(Texture *texture, GLenum internal_format, int width, int height);
