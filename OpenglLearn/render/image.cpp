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

//std::map<const std::string, unsigned int> Image::s_map_texture_cache;

Image::Image() {
    
}

void Image::setData() {
    float vertices[] = {
        // positions         // texture coords
        -0.5f,  1.f, 0.0f,  1.0f, 1.0f, // top right
        -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -1.f, 0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.f,  1.f, 0.0f, 0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Image::~Image() {
}

bool Image::setPath(const char* filepath) {
    _texture = TextureFromFile(std::string(filepath));
    return true;
}

bool Image::draw() {
    auto shader = ShaderCache::GetInstance().GetShader(m_type);
    if (!shader) {
        return false;
    }
    
    if (_texture == 0) {
        return false;
    }
    
    setData();
    
    shader->use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    if (m_type == ShaderType::Image) {
        Matrix model;
        shader->setMat4("uMvp",model);
    }
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return true;
}

void Image::loadTexture(const std::string &filename) {
    if (filename.empty()) {
        return;
    }
    {
        std::lock_guard<std::mutex> gurad(_image_mutex);
        auto iter = s_map_image.find(filename);
        if (iter != s_map_image.end()) {
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
        std::lock_guard<std::mutex> gurad(_image_mutex);
        s_map_image[filename] = tex;
    }
}

unsigned int Image::genTexture(const Texture& tex) {
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
        
        s_map_texture_cache[tex.filepath] = textureID;
    }
    return textureID;
}

unsigned int Image::TextureFromFile(const std::string &filename)
{
    // 先查s_map_texture_cache
    auto iter = s_map_texture_cache.find(filename);
    if (iter != s_map_texture_cache.end()) {
        return iter->second;
    }

    // 再查image
    {
        std::lock_guard<std::mutex> gurad(_image_mutex);
        auto iter = s_map_image.find(filename);
        if (iter != s_map_image.end()) {
            return genTexture(iter->second);
        }
    }
    
    // 最后再读文件
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    // stbi_load_16_from_memory
    if (data)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        ImageBuffer pData = std::make_shared<std::vector<unsigned char>>();
        pData->assign(data, data + width * height * nrComponents);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData->data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        
        s_map_texture_cache[filename] = textureID;
        return textureID;
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    return -1;
}

unsigned int Image::TextureFromMem(unsigned char* buffer, int len) {
    unsigned int textureID = -1;
    int width, height, nrComponents;
    unsigned short *data = stbi_load_16_from_memory(buffer, len, &width, &height, &nrComponents, 0);
    // stbi_load_16_from_memory
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        //std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
        assert(0);
    }

    return textureID;
}
