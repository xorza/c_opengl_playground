#include "shader.hpp"


#include <new>
#include <cstdio>

Shader::~Shader() {
    if (id) glDeleteShader(id);
}

int create_shader(Shader *shader, const char *source, GLenum shader_type) {
    new(shader) Shader();

    shader->id = glCreateShader(shader_type);
    glShaderSource(shader->id, 1, &source, nullptr);
    glCompileShader(shader->id);

    int success = 1;
    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader->id, 512, nullptr, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
        return -1;
    }

    return 0;
}

ShaderProgram::~ShaderProgram() {
    if (id) glDeleteProgram(id);
}

void ShaderProgram::bind() {
    glUseProgram(id);
}

void ShaderProgram::set_uniform(const std::string &name, const glm::vec4 &value) {
    auto it = uniforms.find(name);
    if (it == uniforms.end()) {
        fprintf(stderr, "Uniform %s not found\n", name.c_str());
        return;
    }

    glProgramUniform4fv(id, it->second.location, 1, &value[0]);
}

void ShaderProgram::set_uniform(const std::string &name, const glm::mat4 &value) {
    auto it = uniforms.find(name);
    if (it == uniforms.end()) {
        fprintf(stderr, "Uniform %s not found\n", name.c_str());
        return;
    }

    glProgramUniformMatrix4fv(id, it->second.location, 1, GL_FALSE, &value[0][0]);
}

int create_shader_program(ShaderProgram *shader_program, Shader *vertex_shader, Shader *fragment_shader) {
    new(shader_program) ShaderProgram();

    shader_program->id = glCreateProgram();
    glAttachShader(shader_program->id, vertex_shader->id);
    glAttachShader(shader_program->id, fragment_shader->id);

    glLinkProgram(shader_program->id);

    int success = 1;
    glGetProgramiv(shader_program->id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program->id, 512, nullptr, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
        return -1;
    }

    //iterate over all uniforms
    int uniform_count = 0;
    glGetProgramiv(shader_program->id, GL_ACTIVE_UNIFORMS, &uniform_count);
    for (int i = 0; i < uniform_count; i++) {
        UniformInfo uniform_info = {};
        uniform_info.name.resize(50);
        int name_length = 0;
        glGetActiveUniform(
                shader_program->id,
                i,
                static_cast<GLsizei>(uniform_info.name.size()),
                &name_length,
                &uniform_info.size,
                &uniform_info.type,
                uniform_info.name.data()
        );
        uniform_info.name.resize(name_length);
        uniform_info.location = glGetUniformLocation(shader_program->id, uniform_info.name.c_str());

        shader_program->uniforms[uniform_info.name] = uniform_info;
    }

    return 0;
}
