#include "renderer.h"

#include "window.h"

#include <stdio.h>


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

int create_shader_program(
        const char *vertex_shader_source,
        const char *fragment_shader_source
) {
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

int create_renderer(struct Renderer *renderer) {
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

    int vertex_shader = create_shader(
            vertex_shader_source,
            GL_VERTEX_SHADER
    );
    if (vertex_shader == -1) {
        return -1;
    }

    int fragment_shader = create_shader(
            fragment_shader_source,
            GL_FRAGMENT_SHADER
    );
    if (fragment_shader == -1) {
        return -1;
    }

    renderer->shader_program = create_shader_program(
            vertex_shader_source,
            fragment_shader_source
    );
    if (renderer->shader_program == -1) {
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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




    glUseProgram(renderer->shader_program);


    // Set the rectangle's position and size
    float rect_x = 100.0f; // replace with your values
    float rect_y = 100.0f; // replace with your values
    float rect_width = 200.0f; // replace with your values
    float rect_height = 200.0f; // replace with your values
    int rect_location = glGetUniformLocation(renderer->shader_program, "uRect");
    glUniform4f(rect_location, rect_x, rect_y, rect_width, rect_height);

    // Set the screen size
    int screen_size_location = glGetUniformLocation(renderer->shader_program, "uScreenSize");
    glUniform2f(screen_size_location, (float) window->width, (float) window->height);

    glBindVertexArray(renderer->rect_va);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}