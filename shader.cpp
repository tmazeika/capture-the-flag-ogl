#include <fstream>
#include <iostream>

#include "shader.h"

std::string read_file(std::string name)
{
    std::string line, result;
    std::ifstream file;
    file.open(name, std::ios::in);

    if (file.is_open()) {
        while (getline(file, line)) {
            result += line + "\n";
        }

        file.close();
    }

    return result;
}

GLuint compile_shader(GLenum shader_type, std::string shader_file_name)
{
    GLint success;
    GLchar info_log[512];

    const GLuint shader = glCreateShader(shader_type);
    const std::string file = read_file(shader_file_name);
    const GLchar *file_c = file.c_str();
    glShaderSource(shader, 1, &file_c, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "Error compiling shader '" << shader_file_name << "'\n"
                  << info_log << std::endl;
    }

    return shader;
}

void Shader::initialize()
{
    GLint success;
    GLchar info_log[512];

    const GLuint vert = compile_shader(GL_VERTEX_SHADER, "vert.glsl");
    const GLuint frag = compile_shader(GL_FRAGMENT_SHADER, "frag.glsl");

    // create and link program
    this->program = glCreateProgram();
    glAttachShader(this->program, vert);
    glAttachShader(this->program, frag);
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(this->program, 512, NULL, info_log);
        std::cout << "Error linking shader program\n" << info_log << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::use()
{
    glUseProgram(this->program);
}