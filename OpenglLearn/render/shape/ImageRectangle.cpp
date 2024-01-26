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

bool ImageRectangle::draw() {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Buffer) * m_buffer.size(), m_buffer.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Ground);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    int texture = Image::TextureFromFile(m_image_path);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("uTexture", 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Scene::GetShadowTexture());
    shader->setInt("uTextureShadowMap", 1);
    
    // 矩阵
    glm::mat4 model(1.f);
    shader->setMat4("uModel", model);
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uVP", mpMatrix);
    shader->setMat4("uLightSpaceMatrix", Scene::GetLightVPMatrix());
    
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
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

void ImageRectangle::calculate() {
    // 1 2 3 4为左下右上的顺序
    auto x = _pos2 - _pos1;
    float xCount = std::max((glm::length(x) / m_step.x), 1.f);
    auto y = _pos3 - _pos2;
    float yCount = std::max((glm::length(y) / m_step.y), 1.f);
    
    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(3);
    m_indices.push_back(1);
    m_indices.push_back(2);
    m_indices.push_back(3);
    // push
    Buffer cur_rect;
    cur_rect.pos = _pos1;
    cur_rect.coord = {0, 0};
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos2;
    cur_rect.coord = {xCount, 0};
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos3;
    cur_rect.coord = {xCount, yCount};
    m_buffer.push_back(cur_rect);
    
    cur_rect.pos = _pos4;
    cur_rect.coord = {0, yCount};
    m_buffer.push_back(cur_rect);
}

ImageRectangle::~ImageRectangle() {
    
}
