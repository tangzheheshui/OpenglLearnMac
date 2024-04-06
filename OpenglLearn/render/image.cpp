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
        return genTexture(iter1->second);
    }
    return -1;
}

void TextureMng::createShadowTexture() {
    std::lock_guard guard(m_mutux);
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

