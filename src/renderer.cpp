#include "renderer.hpp"

#include "window.hpp"

#include <cstdio>
#include <cassert>
#include <glm.hpp>
#include <new>

int create_shader(const char *source, GLenum shader_type) {
    int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
        return -1;
    }

    return shader;
}

int create_shader_program() {
    const char *vertex_shader_source =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform vec4 uRect; // x, y, width, height\n"
            "uniform vec2 uScreenSize; // screen width, screen height\n"
            "void main() {\n"
            "   // Normalize position to [-1, 1] range for rendering\n"
            "   float nx = 2.0 * (aPos.x * uRect.z + uRect.x) / uScreenSize.x - 1.0;\n"
            "   float ny = 2.0 * (aPos.y * uRect.w + uRect.y) / uScreenSize.y - 1.0;\n"
            "   gl_Position = vec4(nx, ny, aPos.z, 1.0);\n"
            "}\n";

    const char *fragment_shader_source =
            "#version 330 core\n"
            "uniform vec4 uColor;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = uColor;\n"
            "}\n";

    int vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
    if (vertex_shader == -1) return -1;

    int fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (fragment_shader == -1) return -1;

    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success = 1;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    return shader_program;
}

int create_renderer(struct Renderer *renderer) {
    new(renderer) Renderer{};

    renderer->shader_program = create_shader_program();
    if (renderer->shader_program == -1) {
        return -1;
    }

    //iterate over all uniforms
    int uniform_count;
    glGetProgramiv(renderer->shader_program, GL_ACTIVE_UNIFORMS, &uniform_count);
    for (int i = 0; i < uniform_count; i++) {
        UniformInfo uniform_info = {};
        uniform_info.name.resize(50);
        int name_length = 0;
        glGetActiveUniform(
                renderer->shader_program,
                i,
                static_cast<GLsizei>(uniform_info.name.size()),
                &name_length,
                &uniform_info.size,
                &uniform_info.type,
                uniform_info.name.data()
        );
        uniform_info.name.resize(name_length);
        uniform_info.location = glGetUniformLocation(renderer->shader_program, uniform_info.name.c_str());

        // Add the uniform to the hash table
        renderer->uniforms[uniform_info.name] = uniform_info;
    }


    float vertices[] = {
            0.0f, 0.0f, 0.0f, // bottom left corner
            1.0f, 0.0f, 0.0f,  // bottom right corner
            0.0f, 1.0f, 0.0f,  // top left corner
            1.0f, 1.0f, 0.0f,   // top right corner
    };

    uint32_t vertex_buffer;
    glGenBuffers(1, &vertex_buffer);

    glGenVertexArrays(1, &renderer->rect_va);
    glBindVertexArray(renderer->rect_va);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    return 0;
}


Renderer::~Renderer() {
    glDeleteProgram(this->shader_program);
    glDeleteVertexArrays(1, &this->rect_va);
}


void render(
        Renderer *renderer,
        Window *window,
        FrameInfo *frame_info
) {
    glViewport(0, 0, window->width, window->height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    UniformInfo *uniform_info;

    glm::vec4 rect1 = {50.0f, 50.0f, (float) window->width - 100.0f, (float) window->height - 100.0f};
    uniform_info = &renderer->uniforms.at("uRect");
    assert(uniform_info != nullptr);
    glProgramUniform4f(renderer->shader_program, uniform_info->location, rect1[0], rect1[1], rect1[2], rect1[3]);

    uniform_info = &renderer->uniforms.at("uScreenSize");
    glProgramUniform2f(renderer->shader_program, uniform_info->location, (float) window->width, (float) window->height);

    auto color1 = glm::vec4(0.5f, 0.2f,1.0f,  1.0f);
    uniform_info = &renderer->uniforms.at("uColor");
    glProgramUniform4f(renderer->shader_program, uniform_info->location, color1[0], color1[1], color1[2], color1[3]);

    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->rect_va);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}
