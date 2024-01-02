//
//  shader.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#include "shader.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

const std::string g_file_path = "/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/render/shaders/";

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
    std::ifstream vertFile;
    std::ifstream fragFile;
    vertFile.open(g_file_path + vertPath);
    fragFile.open(g_file_path + fragPath);
    std::string strVert;
    std::string strFrag;
    if (vertFile.is_open() && fragFile.is_open()) {
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vertFile.rdbuf();
        fShaderStream << fragFile.rdbuf();
        strVert = vShaderStream.str();
        strFrag = fShaderStream.str();
        vertFile.close();
        fragFile.close();
    } else {
        return;
    }

    const char* strVertex = strVert.c_str();
    const char* strFragment = strFrag.c_str();
    
    int success = 0;
    char infoLog[512] = {0};
    
    // vertex
    unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(iVertexShader, 1, &strVertex, nullptr);
    glCompileShader(iVertexShader);
    glGetShaderiv(iVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(iVertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;    }
    
    // fragment
    unsigned int iFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(iFragment, 1, &strFragment, nullptr);
    glCompileShader(iFragment);
    glGetShaderiv(iFragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(iFragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // program
    _ID = glCreateProgram();
    glAttachShader(_ID, iVertexShader);
    glAttachShader(_ID, iFragment);
    glLinkProgram(_ID);
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // del
    glDeleteShader(iVertexShader);
    glDeleteShader(iFragment);
}

Shader::~Shader() {
    glDeleteProgram(_ID);
}

void Shader::use() {
    glUseProgram(_ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

// 目前存在多线程问题，暂不考虑
ShaderCache& ShaderCache::GetInstance() {
    static ShaderCache instance;
    return instance;
}

ShaderCache::ShaderCache() {
    if (auto shader = new Shader("vert_image", "frag_image")) {
        m_map_shader.insert(std::make_pair(ShaderType::Image, shader));
    }
}

Shader* ShaderCache::GetShader(ShaderType type) {
    auto iter = m_map_shader.find(type);
    if (iter != m_map_shader.end()) {
        return iter->second;
    }
    return nullptr;
}

