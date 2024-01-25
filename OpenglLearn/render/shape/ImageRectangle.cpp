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

void ImageRectangle::draw() {
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
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Image);
    if (!shader) {
        return ;
    }
    
    shader->use();
    
    int texture = Image::TextureFromFile(m_image_path);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("uTexture", 0);
    
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uMvp", mpMatrix);
    
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
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
    float lenx = glm::length(x);
    int xCount = std::max((int)(lenx / m_step.x), 1);
    auto y = _pos3 - _pos2;
    float leny = glm::length(y);
    int yCount = std::max((int)(leny / m_step.y), 1);
    
    glm::vec3 setp_x = {x.x/xCount, x.y/xCount, x.z/xCount};
    glm::vec3 setp_y = {y.x/yCount, y.y/yCount, y.z/yCount};
    int index = 0;
    
    glm::vec3 vec1, vec2, vec3, vec4; 
    Buffer cur_rect;
    for (int i = 0; i < xCount; i++) {
        vec1 = _pos1 + glm::vec3(setp_x.x * i, setp_x.y * i, setp_x.z * i);
        index = i+1;
        vec2 = _pos1 + glm::vec3(setp_x.x * index, setp_x.y * index, setp_x.z * index);
        if (index == xCount) {
            vec2 = _pos2;
        } 
        
        vec3 = vec2;
        vec4 = vec1;
        
        for (int j = 0; j < yCount; j++) {
            // 索引
            int cur_index = (int)m_buffer.size();
            m_indices.push_back(cur_index);
            m_indices.push_back(cur_index+1);
            m_indices.push_back(cur_index+3);
            m_indices.push_back(cur_index+1);
            m_indices.push_back(cur_index+2);
            m_indices.push_back(cur_index+3);
            // push
            cur_rect.pos = vec4;
            cur_rect.coord = {0, 0};
            m_buffer.push_back(cur_rect);
            
            cur_rect.pos = vec3;
            cur_rect.coord = {1, 0};
            m_buffer.push_back(cur_rect);
            
            printf("i = %d, j = %d, p1=(%f, %f)\n", i, j, vec4.x, vec4.z);
            printf("i = %d, j = %d, p2=(%f, %f)\n", i, j, vec3.x, vec3.z);
            
            // 
            vec3 += glm::vec3(setp_y.x, setp_y.y, setp_y.z);
            vec4 += glm::vec3(setp_y.x, setp_y.y, setp_y.z);
            if ((j+1) == yCount) {
                vec4 = vec1 + (_pos4 - _pos1);
                vec3 = vec2 + (_pos4 - _pos1);
            }
            cur_rect.pos = vec3;
            cur_rect.coord = {1, 1};
            m_buffer.push_back(cur_rect);
            
            cur_rect.pos = vec4;
            cur_rect.coord = {0, 1};
            m_buffer.push_back(cur_rect);
            
            printf("i = %d, j = %d, p3=(%f, %f)\n", i, j, vec3.x, vec3.z);
            printf("i = %d, j = %d, p4=(%f, %f)\n", i, j, vec4.x, vec4.z);
        }
    }
}

ImageRectangle::~ImageRectangle() {
    
}
