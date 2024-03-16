//
//  ImageRectangle.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#include "ImageRectangle.hpp"
#include "../image.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../scene.hpp"
#include "../Light.h"

bool ImageRectangle::draw() {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    int texture_normal = Image::TextureFromFile(m_image_normal);
    bool has_normal = (texture_normal > 0);
    
    int texture_height = Image::TextureFromFile(m_image_height);
    bool has_height = (texture_height > 0);
    
    if (!_has_bind_vbo) {
        glBindVertexArray(_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Buffer) * m_buffer.size(), m_buffer.data(), GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // tangent
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        // bitangent
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);
        
        glBindVertexArray(0);
        _has_bind_vbo = true;
    }
    
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Ground);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    int texture = Image::TextureFromFile(m_image_diffuse);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("uTexture", 0);
    
    int indexTex = 1;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Scene::GetShadowTexture());
    shader->setInt("uTextureShadowMap", indexTex++);
    
    if (has_normal) {
        glActiveTexture(GL_TEXTURE0 + indexTex);
        glBindTexture(GL_TEXTURE_2D, texture_normal);
        shader->setInt("uTextureNormal", indexTex++);
        shader->setInt("uHasNormal", 1);
    } else {
        shader->setInt("uHasNormal", 0);
    }
    
    if (has_height) {
        glActiveTexture(GL_TEXTURE0 + indexTex);
        glBindTexture(GL_TEXTURE_2D, texture_height);
        shader->setInt("uTextureHeight", indexTex++);
        shader->setInt("uHasHeight", 1);
    } else {
        shader->setInt("uHasHeight", 0);
    }
    
    // 矩阵
    Matrix model;
    shader->setMat4("uModel", model);
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uVP", mpMatrix);
    shader->setMat4("uLightSpaceMatrix", Scene::GetLightVPMatrix());
    
    // 灯光
    Scene scene = Scene::getScene();
    scene.setLightUniform(shader);
    
    // 相机位置
    // auto cam_pos = Camera::GetCamera().getPossition();
    shader->setFloat3("uCameraPos", 15, 15, 0);
    
    // 透明度
    shader->setFloat("uAlpha", getAlpha());
    
    // 开启混合
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, (int)m_buffer.size());
    return true;
}

void ImageRectangle::setPoints(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &pos4) {
    _pos1 = pos1;
    _pos2 = pos2;
    _pos3 = pos3;
    _pos4 = pos4;
}

void ImageRectangle::setSetp(float w, float h) {
    m_step = {w, h};
}

void ImageRectangle::getTangent(const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3, const glm::vec2 &uv4, glm::vec3 &tangent1, glm::vec3 &bitangent1, glm::vec3 &tangent2, glm::vec3 &bitangent2) {
    glm::vec3 edge1 = _pos2 - _pos1;
    glm::vec3 edge2 = _pos3 - _pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;
    
    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    
    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);
    
    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);
    
    // - triangle 2
    edge1 = _pos3 - _pos1;
    edge2 = _pos4 - _pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;
    
    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    
    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);
    
    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);
}

void ImageRectangle::calculate() {
    // 1 2 3 4为左下右上的顺序
    auto x = _pos2 - _pos1;
    float xCount = std::max((glm::length(x) / m_step.x), 1.f);
    auto y = _pos3 - _pos2;
    float yCount = std::max((glm::length(y) / m_step.y), 1.f);
    
    glm::vec2 uv1 = {0, 0};
    glm::vec2 uv2 = {xCount, 0};
    glm::vec2 uv3 = {xCount, yCount};
    glm::vec2 uv4 = {0, yCount};
    
    glm::vec3 tangent1;
    glm::vec3 tangent2;
    glm::vec3 bitangent1;
    glm::vec3 bitangent2;
    
    getTangent(uv1, uv2, uv3, uv4, tangent1, bitangent1, tangent2, bitangent2);
    
    auto normal = glm::cross(_pos2 - _pos1, _pos4 - _pos1);
    Buffer cur_rect;
    // 1 2 3
    cur_rect.normal = normal;
    cur_rect.tangent = tangent1;
    cur_rect.bitangent = bitangent1;
    cur_rect.pos = _pos1;
    cur_rect.coord = uv1;
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos2;
    cur_rect.coord = uv2;
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos3;
    cur_rect.coord = uv3;
    m_buffer.push_back(cur_rect);
    
    // 1 3 4
    cur_rect.tangent = tangent2;
    cur_rect.bitangent = bitangent2;
    cur_rect.pos = _pos1;
    cur_rect.coord = uv1;
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos3;
    cur_rect.coord = uv3;
    m_buffer.push_back(cur_rect);
    cur_rect.pos = _pos4;
    cur_rect.coord = uv4;
    m_buffer.push_back(cur_rect);
}

ImageRectangle::~ImageRectangle() {
    
}

void ImageRectangle::setImagePath(const std::string &path, const std::string &diffuseMap, const std::string &normalMap, const std::string &heightMap) {
    m_image_diffuse = path + diffuseMap;
    m_image_normal = path + normalMap;
    m_image_height = path + heightMap;
}
