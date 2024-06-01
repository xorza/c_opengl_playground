#pragma once

#include "../utils/gl_includes.hpp"
#include "../utils/nocopy.hpp"
#include "../utils/glm_includes.hpp"

#include <cstdint>
#include <map>
#include <string>


struct Shader {
    NOCOPY(Shader)

    GLuint id = 0;

    Shader() = default;
    ~Shader();
};

int create_shader(Shader *shader, const char *source, GLenum shader_type);

struct UniformInfo {
    std::string name;
    int location;
    int size;
    GLenum type;
};


struct ShaderProgram {
    NOCOPY(ShaderProgram)

    GLuint id = 0;

    std::map<std::string, UniformInfo> uniforms;

    ShaderProgram() = default;
    ~ShaderProgram();

    void bind();
    void set_uniform(  const std::string& name, const glm::vec4 &value);
    void set_uniform( const std::string &name, const glm::mat4 &value);

};

int create_shader_program(ShaderProgram *shader_program, Shader *vertex_shader, Shader *fragment_shader);