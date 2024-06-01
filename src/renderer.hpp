#pragma once

#include "ogl/vao.hpp"
#include "ogl/shader.hpp"
#include "ogl/texture.hpp"

#include "utils/gl_includes.hpp"
#include "utils/nocopy.hpp"

#include <cstdint>
#include <map>
#include <string>


struct Renderer {
    NOCOPY(Renderer)

    Shader vertex_shader;
    ShaderProgram shader;
    Vao vao;
    Texture texture;



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