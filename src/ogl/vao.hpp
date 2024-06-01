#pragma once


#include "../utils/gl_includes.hpp"
#include "../utils/nocopy.hpp"

#include <cstdint>


struct Vao {
    NOCOPY(Vao)

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    Vao() = default;

    ~Vao();

    template<typename Vert>
    void attach_vbo(Vert *vertices, uint32_t count) {
        attach_vbo(vertices, sizeof(Vert), count);
    }

    void attach_vbo(void *vertices, int32_t stride, int32_t count);

    void attach_ebo(uint16_t *indices, int32_t count);

    void enable_attrib(int index, int size, GLenum type, int offset) const;

    void bind() const;
};

int create_vao(Vao *vao);
