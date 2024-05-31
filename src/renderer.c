#include "renderer.h"

#include "window.h"

#include <stdio.h>
#include <uthash.h>
#include <assert.h>
#include <cglm/cglm.h>

int create_shader(const char *source, GLenum shader_type) {
    int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
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
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n";

    int vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
    if (vertex_shader == -1) return -1;

    int fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (fragment_shader == -1) return -1;

    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    return shader_program;
}

int create_renderer(struct Renderer *renderer) {
    renderer->shader_program = create_shader_program();
    if (renderer->shader_program == -1) {
        return -1;
    }

    //iterate over all uniforms
    int uniform_count;
    glGetProgramiv(renderer->shader_program, GL_ACTIVE_UNIFORMS, &uniform_count);
    for (int i = 0; i < uniform_count; i++) {
        struct UniformInfo *const uniform_info = malloc(sizeof(struct UniformInfo));
        glGetActiveUniform(
                renderer->shader_program,
                i,
                sizeof(uniform_info->name),
                NULL,
                &uniform_info->size,
                &uniform_info->type,
                uniform_info->name
        );
        uniform_info->location = glGetUniformLocation(renderer->shader_program, uniform_info->name);

        // Add the uniform to the hash table
        HASH_ADD_STR(renderer->uniforms, name, uniform_info);
    }


    float vertices[] = {
            0.0f, 0.0f, 0.0f, // bottom left corner
            1.0f, 0.0f, 0.0f,  // bottom right corner
            0.0f, 1.0f, 0.0f,  // top left corner
            1.0f, 1.0f, 0.0f,   // top right corner
    };

    uint32_t vertex_buffer;
    glGenBuffers(1, &vertex_buffer);

    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    renderer->rect_va = vertex_array;

    return 0;
}

void destroy_renderer(struct Renderer *renderer) {
    struct UniformInfo *item, *tmp;

    HASH_ITER(hh, renderer->uniforms, item, tmp) {
        HASH_DEL(renderer->uniforms, item);
        free(item);
    }

    glDeleteProgram(renderer->shader_program);
    glDeleteVertexArrays(1, &renderer->rect_va);
}

void render(
        struct Renderer *renderer,
        struct Window *window,
        struct FrameInfo *frame_info
) {
    glViewport(0, 0, window->width, window->height);
    glClearColor(0.1f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    struct UniformInfo *uniform_info;

    // Set the rectangle's position and size
    vec4 rect = {100.0f, 100.0f, 200.0f, 200.0f};

    HASH_FIND_STR(renderer->uniforms, "uRect", uniform_info);
    assert(uniform_info != NULL);
    glProgramUniform4f(renderer->shader_program, uniform_info->location, rect[0], rect[1], rect[2], rect[3]);

    // Set the screen size
    HASH_FIND_STR(renderer->uniforms, "uScreenSize", uniform_info);
    assert(uniform_info != NULL);
    glProgramUniform2f(renderer->shader_program, uniform_info->location, (float) window->width, (float) window->height);


    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->rect_va);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}