#include "vao.hpp"

#include <new>

Vao::~Vao() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
        if (vbo)
            glDeleteBuffers(1, &vbo);
        if (ebo)
            glDeleteBuffers(1, &ebo);
    }
}

void Vao::attach_vbo(void *vertices, int32_t stride, int32_t count) {
    glCreateBuffers(1, &vbo);

    glNamedBufferData(vbo, stride * count, vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, stride);

}

void Vao::attach_ebo(uint16_t *indices, int32_t count) {
    glCreateBuffers(1, &ebo);

    glNamedBufferData(ebo, sizeof(uint16_t) * count, indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ebo);
}

void Vao::enable_attrib(int index, int size, GLenum type, int offset) const {
    glEnableVertexArrayAttrib(vao, index);
    glVertexArrayAttribFormat(vao, index, size, type, GL_FALSE, offset);
    glVertexArrayAttribBinding(vao, index, 0);
}

void Vao::bind() const {
    glBindVertexArray(vao);
}

int create_vao(Vao *vao) {
    new(vao) Vao();

    glCreateVertexArrays(1, &vao->vao);

    return 0;
}