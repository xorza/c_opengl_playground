#pragma once

#include "gl_includes.h"

#include <inttypes.h>

struct Renderer {
    int shader_program;
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