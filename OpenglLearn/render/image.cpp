//
//  image.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#include "image.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../third/stb_image.h"

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

TextureMng::TextureMng() {
    createShadowTexture();
}

void TextureMng::loadTexture(const std::string &filename) {
    if (filename.empty()) {
        return;
    }
    
    {
        std::lock_guard<std::mutex> gurad(m_mutux);
        auto iter = m_map_tetures.find(filename);
        if (iter != m_map_tetures.end()) {
            return;
        }
    }
    
    Texture tex;
    unsigned char *data = stbi_load(filename.c_str(), &tex.width, &tex.height, &tex.format, 0);
    ImageBuffer pData = std::make_shared<std::vector<unsigned char>>();
    pData->assign(data, data + tex.width * tex.height * tex.format);
    tex.data = std::move(pData);
    tex.filepath = filename;
    {
        std::lock_guard<std::mutex> gurad(m_mutux);
        m_map_tex_data[filename] = tex;
    }
}

unsigned int TextureMng::getTexture(const std::string &filename) {
    std::lock_guard guard(m_mutux);
    auto iter = m_map_tetures.find(filename);
    if (iter != m_map_tetures.end()) {
        return iter->second;
    }
    
    auto iter1 = m_map_tex_data.find(filename);
    if (iter1 != m_map_tex_data.end()) {
        auto tex = genTexture(iter1->second);
        
        // 清掉临时内存
        m_map_tex_data.erase(iter1);
        return tex;
    }
    return -1;
}

void TextureMng::createShadowTexture() {
    // 创建阴影贴图
    GLuint _depthTexture;
    glGenTextures(1, &_depthTexture);
    glBindTexture(GL_TEXTURE_2D, _depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 
                 0,
                 GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, 
                 SHADOW_HEIGHT,
                 0, 
                 GL_DEPTH_COMPONENT, 
                 GL_FLOAT, 
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    std::lock_guard guard(m_mutux);
    m_map_tetures[STR_DEPTH_TEXTURE] = _depthTexture;
}

void TextureMng::createSkyTexture() {
    
}

unsigned int TextureMng::genTexture(const Texture& tex) {
    unsigned int textureID = -1;
    if (!tex.data->empty()) {
        glGenTextures(1, &textureID);
        
        GLenum format = 0;
        if (tex.format == 1)
            format = GL_RED;
        else if (tex.format == 3)
            format = GL_RGB;
        else if (tex.format == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.data->data());
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        m_map_tetures[tex.filepath] = textureID;
    }
    return textureID;
}

unsigned int TextureMng::getCubeTexture(std::array<std::string, 6> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        auto file = STR_TEXTURE_PATH + faces[i];
        unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return -1;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
