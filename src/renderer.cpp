#include "renderer.hpp"

#include "window.hpp"

#include "utils/glm_includes.hpp"

#include <cstdio>
#include <cassert>
#include <new>


int create_renderer(Renderer *renderer) {
    new(renderer) Renderer{};


    const char *vertex_shader_source = R"glsl(
            #version 420 core
            uniform mat4 uMvp;
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTex;
            out vec2 texCoord;
            void main() {
               vec4 pos = vec4(aPos, 1.0);
               texCoord = aTex;
               gl_Position = uMvp * pos;
            }
    )glsl";
    const char *fragment_shader_source = R"glsl(
            #version 420 core
            uniform vec4 uColor;
            layout (binding = 0) uniform sampler2D uTexture;
            in vec2 texCoord;
            out vec4 FragColor;
            void main() {
               FragColor = texture(uTexture, texCoord) * uColor;
            }
    )glsl";
    create_shader(&renderer->vertex_shader, vertex_shader_source, GL_VERTEX_SHADER);
    Shader fragment_shader;
    create_shader(&fragment_shader, fragment_shader_source, GL_FRAGMENT_SHADER);
    create_shader_program(&renderer->shader, &renderer->vertex_shader, &fragment_shader);


    uint8_t texture_data[] = {
            255, 0, 0, 255,
            0, 255, 0, 255,
            255, 255, 0, 255,
            0, 0, 0, 255,
    };
    create_texture(&renderer->texture, GL_RGBA8, 2, 2);
    renderer->texture.upload(texture_data);


    struct Vertex {
        GLfloat x, y, z;     // Position
        GLfloat u, v;        // Texture coordinates
    };
    Vertex vertices[] = {
            // Front face
            {-1.0f, -1.0f, 1.0f,  0.0f, 0.0f},
            {1.0f,  -1.0f, 1.0f,  1.0f, 0.0f},
            {1.0f,  1.0f,  1.0f,  1.0f, 1.0f},
            {-1.0f, 1.0f,  1.0f,  0.0f, 1.0f},
            // Back face
            {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
            {1.0f,  -1.0f, -1.0f, 1.0f, 0.0f},
            {1.0f,  1.0f,  -1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f,  -1.0f, 0.0f, 1.0f},
            // Left face
            {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
            {-1.0f, -1.0f, 1.0f,  1.0f, 0.0f},
            {-1.0f, 1.0f,  1.0f,  1.0f, 1.0f},
            {-1.0f, 1.0f,  -1.0f, 0.0f, 1.0f},
            // Right face
            {1.0f,  -1.0f, -1.0f, 0.0f, 0.0f},
            {1.0f,  -1.0f, 1.0f,  1.0f, 0.0f},
            {1.0f,  1.0f,  1.0f,  1.0f, 1.0f},
            {1.0f,  1.0f,  -1.0f, 0.0f, 1.0f},
            // Top face
            {-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f},
            {-1.0f, 1.0f,  1.0f,  1.0f, 0.0f},
            {1.0f,  1.0f,  1.0f,  1.0f, 1.0f},
            {1.0f,  1.0f,  -1.0f, 0.0f, 1.0f},
            // Bottom face
            {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f},
            {-1.0f, -1.0f, 1.0f,  1.0f, 0.0f},
            {1.0f,  -1.0f, 1.0f,  1.0f, 1.0f},
            {1.0f,  -1.0f, -1.0f, 0.0f, 1.0f},
    };
    uint16_t indices[] = {
            0, 1, 2, 2, 3, 0,
            6, 5, 4, 4, 7, 6,
            8, 9, 10, 10, 11, 8,
            14, 13, 12, 12, 15, 14,
            16, 17, 18, 18, 19, 16,
            22, 21, 20, 20, 23, 22,
    };
    create_vao(&renderer->vao);
    renderer->vao.attach_vbo(vertices, sizeof(vertices) / sizeof(Vertex));
    renderer->vao.attach_ebo(indices, sizeof(indices) / sizeof(uint16_t));
    renderer->vao.enable_attrib(0, 3, GL_FLOAT, offsetof(Vertex, x));
    renderer->vao.enable_attrib(1, 2, GL_FLOAT, offsetof(Vertex, u));


    return 0;
}


Renderer::~Renderer() = default;


void render(
        Renderer *renderer,
        Window *window,
        FrameInfo *frame_info
) {
    glViewport(0, 0, window->width, window->height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    auto mvp =
            glm::perspective(
                    glm::radians(90.0f),
                    (float) window->width / (float) window->height,
                    0.1f, 100.0f
            )
            *
            glm::lookAt(
                    glm::vec3(0.0f, 0.0f, 3.7f),
                    glm::vec3(0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)
            )
            *
            glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(frame_info->time * 50.0f),
                    glm::normalize(glm::vec3(0.1f, glm::clamp(frame_info->time * 0.1f, 0.0f, 1.0f), 0.0f))
            );


    auto color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    renderer->shader.set_uniform("uMvp", mvp);
    renderer->shader.set_uniform("uColor", color1);

    renderer->texture.bind(0);
    renderer->shader.bind();
    renderer->vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);


}
