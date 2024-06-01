#include "renderer.hpp"

#include "window.hpp"

#include "utils/glm_includes.hpp"

#include <cstdio>
#include <cassert>
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


    uint8_t texture_data[] = {
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255,
            255, 255, 0, 255,
    };
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer->texture);
    glTextureStorage2D(renderer->texture, 1, GL_RGB8, 2, 2);
    glTextureSubImage2D(renderer->texture, 0, 0, 0, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, &texture_data);
    glTextureParameteri(renderer->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(renderer->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderer->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

    GLuint indices[] = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            6, 5, 4, 4, 7, 6,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            14, 13, 12, 12, 15, 14,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            22, 21, 20, 20, 23, 22,
    };

    GLuint VBO, EBO;
    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &renderer->cube_va);
    glBindVertexArray(renderer->cube_va);

    // Generate and bind the Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the Element Buffer Object (EBO)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);

    // Define the texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);

    return 0;
}


Renderer::~Renderer() {
    glDeleteProgram(this->shader_program);
    glDeleteVertexArrays(1, &this->cube_va);
}


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


    UniformInfo *uniform_info;

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

    uniform_info = &renderer->uniforms.at("uMvp");
    assert(uniform_info != nullptr);
    glProgramUniformMatrix4fv(renderer->shader_program, uniform_info->location, 1, false, &mvp[0][0]);


    auto color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    uniform_info = &renderer->uniforms.at("uColor");
    glProgramUniform4f(renderer->shader_program, uniform_info->location, color1[0], color1[1], color1[2], color1[3]);

//    uniform_info = &renderer->uniforms.at("uTexture");
//    glProgramUniform1i(renderer->shader_program, uniform_info->location, 0);


    glBindTextureUnit(0, renderer->texture);
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->cube_va);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


}
