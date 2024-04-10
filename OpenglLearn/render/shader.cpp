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

Shader::Shader(const std::string &vertPath, const std::string &fragPath, const std::string &geomPath) {
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;    
        assert(0);
    }
    
    // fragment
    unsigned int iFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(iFragment, 1, &strFragment, nullptr);
    glCompileShader(iFragment);
    glGetShaderiv(iFragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(iFragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        assert(0);
    }
    
    _ID = glCreateProgram();
    
    if (!geomPath.empty()) {
        std::ifstream geomFile;
        geomFile.open(g_file_path + geomPath);
        std::string str;
        if (geomFile.is_open()) {
            std::stringstream vShaderStream;
            vShaderStream << geomFile.rdbuf();
            std::string strVert = vShaderStream.str();
            const char* str = strVert.c_str();
            
            // geom
            unsigned int iGepom = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(iGepom, 1, &str, nullptr);
            glCompileShader(iGepom);
            glGetShaderiv(iGepom, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(iGepom, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                assert(0);
            }
            glAttachShader(_ID, iGepom);
            geomFile.close();
        } 
    }
    // program
    glAttachShader(_ID, iVertexShader);
    glAttachShader(_ID, iFragment);
    glLinkProgram(_ID);
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(0);
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

void Shader::setFloat3(const std::string &name, float v1, float v2, float v3) const {
    glUniform3f(glGetUniformLocation(_ID, name.c_str()), v1, v2, v3);
}

void Shader::setFloat4(const std::string &name, float v1, float v2, float v3, float v4) const {
    glUniform4f(glGetUniformLocation(_ID, name.c_str()), v1, v2, v3 , v4);
}

void Shader::setMat4(const std::string &name, const Matrix &value) const {
    glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, value.buffer());
}

void Shader::setMat4Array(const std::string &name, const std::vector<Matrix> &value) const {
    assert(value.size() > 1);
    int loc = glGetUniformLocation(_ID, name.c_str());
    glUniformMatrix4fv(loc, (int)value.size(), GL_FALSE, value.at(0).buffer());
}

// 目前存在多线程问题，暂不考虑
ShaderCache& ShaderCache::GetInstance() {
    static ShaderCache instance;
    return instance;
}

ShaderCache::ShaderCache() {
    if (auto shader = new Shader("vert_color", "frag_color")) {
        m_map_shader.insert(std::make_pair(ShaderType::Color, shader));
    }
    
    if (auto shader = new Shader("vert_image", "frag_image")) {
        m_map_shader.insert(std::make_pair(ShaderType::Image, shader));
    }
    
    if (auto shader = new Shader("vert_model", "frag_model")) {
        m_map_shader.insert(std::make_pair(ShaderType::Model_Texture, shader));
    }
    
    if (auto shader = new Shader("vert_model_color", "frag_model_color")) {
        m_map_shader.insert(std::make_pair(ShaderType::Model_Color, shader));
    }
    
    if (auto shader = new Shader("vert_shadow_color", "frag_shadow")) {
        m_map_shader.insert(std::make_pair(ShaderType::Shadow_Color, shader));
    }
    
    if (auto shader = new Shader("vert_shadow", "frag_shadow")) {
        m_map_shader.insert(std::make_pair(ShaderType::Shadow_Texture, shader));
    }
    
    if (auto shader = new Shader("vert_deepImage", "frag_deepImage")) {
        m_map_shader.insert(std::make_pair(ShaderType::Debug_DeepTexture, shader));
    }
    
    if (auto shader = new Shader("vert_ground_normal", "frag_ground_normal")) {
        m_map_shader.insert(std::make_pair(ShaderType::Ground, shader));
    }
    
    if (auto shader = new Shader("vert_model_color_anim", "frag_model_color")) {
        m_map_shader.insert(std::make_pair(ShaderType::Model_Color_Anim, shader));
    }
    
    if (auto shader = new Shader("vert_model_anim", "frag_model")) {
        m_map_shader.insert(std::make_pair(ShaderType::Model_Texture_Anim, shader));
    }
    
    if (auto shader = new Shader("vert_sky", "frag_sky")) {
        m_map_shader.insert(std::make_pair(ShaderType::Sky, shader));
    }
    
    if (auto shader = new Shader("model_multiview.vert", "frag_model", "model_multiview.geom")) {
        m_map_shader.insert(std::make_pair(ShaderType::Model_Texture_MultiView, shader));
    }
}

Shader* ShaderCache::GetShader(ShaderType type) {
    auto iter = m_map_shader.find(type);
    if (iter != m_map_shader.end()) {
        return iter->second;
    }
    return nullptr;
}

