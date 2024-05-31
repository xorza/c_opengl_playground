#pragma once

#include "gl_includes.hpp"

#include "nocopy.hpp"

#include <cstdint>
#include <map>
#include <string>

struct UniformInfo {
    std::string name;
    int location;
    int size;
    GLenum type;
};

struct Renderer {
    NO_COPY(Renderer)

    int shader_program;
    std::map<std::string, UniformInfo> uniforms;

    uint32_t rect_va;

    Renderer() = default;

    ~Renderer();
};


int create_renderer(Renderer *renderer);


struct Window;
struct FrameInfo;

void render(
        Renderer *renderer,
        Window *window,
        FrameInfo *frame_info
);