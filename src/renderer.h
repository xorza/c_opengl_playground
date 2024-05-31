#pragma once

#include "gl_includes.h"

#include <uthash.h>
#include <inttypes.h>

struct UniformInfo {
    char name[50];
    int location;
    int size;
    GLenum type;
    UT_hash_handle hh;
};

struct Renderer {
    int shader_program;
    struct UniformInfo *uniforms;

    uint32_t rect_va;

};


int create_renderer(struct Renderer *renderer);

void destroy_renderer(struct Renderer *renderer);


struct Window;
struct FrameInfo;

void render(
        struct Renderer *renderer,
        struct Window *window,
        struct FrameInfo *frame_info
);