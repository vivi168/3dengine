#pragma once

#include <GL/gl3w.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
private:
    GLuint program = 0;

    void check_compile_errors(unsigned int shader, std::string type)
    {
        int success;
        char info_log[1024];

        if (type == "PROGRAM")
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
        else
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "Error: " << type << std::endl << info_log << std::endl;
        }
    }
public:
    void load(const std::string vertex_path, const std::string fragment_path)
    {
        // read shaders files
        std::string vertex_content, fragment_content;
        std::ifstream vertex_file, fragment_file;

        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertex_file.open(vertex_path);
            fragment_file.open(fragment_path);

            std::stringstream vertex_stream, fragment_stream;
            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();

            vertex_file.close();
            fragment_file.close();

            vertex_content = vertex_stream.str();
            fragment_content = fragment_stream.str();
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "Error reading file: " << e.what() << std::endl;
        }

        // compile shaders
        const char *vertex_code, *fragment_code;
        unsigned int vertex, fragment;
        
        vertex_code = vertex_content.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_code, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");
        
        fragment_code = fragment_content.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_code, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        check_compile_errors(program, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(program);
    }

    void unlink()
    {
        glDeleteProgram(program);
    }

    GLuint id() const
    {
        return program;
    }
};