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

std::shared_ptr<std::vector<char>> Image::GetTextureDataFromFile(const std::string &filename) {
    if (filename.empty()) {
        return nullptr;
    }
    
    // 读取图片文件的二进制数据到内存中
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open image file." << std::endl;
        return nullptr;
    }
    
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::shared_ptr<std::vector<char>> pData = std::make_shared<std::vector<char>>(fileSize);
    file.read(pData->data(), fileSize);
    
    file.close();
    
    return pData;
}

unsigned int Image::TextureFromFile(const std::string &filename)
{
    auto iter = s_map_texture_cache.find(filename);
    if (iter != s_map_texture_cache.end()) {
        return iter->second;
    }

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
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
