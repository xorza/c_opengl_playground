#include "texture.hpp"

#include <new>

Texture::~Texture() {
    if (id)
        glDeleteTextures(1, &id);

}

void Texture::bind(uint32_t unit) {
    glBindTextureUnit(unit, id);
}

void Texture::upload(const void *data) {
    glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}


int create_texture(Texture *texture, GLenum internal_format, int width, int height) {
    new(texture) Texture{};

    texture->width = width;
    texture->height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &texture->id);
    glTextureStorage2D(texture->id, 1, internal_format, width, height);

    glTextureParameteri(texture->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 0;
}
