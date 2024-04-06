//
//  Line.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#include "Line.hpp"
#include <glad/glad.h>
#include "../shader.hpp"
#include "../camera.hpp"

void Line::setData(const std::vector<glm::vec3> &points, const std::vector<unsigned int> &indices) {
    m_points = points;
    m_indices = indices;
}

bool Line::draw() {
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Color);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    calculate();
    
    // 矩阵
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uMatrixMVP", mpMatrix);
    
    // 颜色
    shader->setFloat3("uColor", m_color.r, m_color.g, m_color.b);
    
    glLineWidth(m_fWidth);
    glDrawElements(GL_LINES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
    return true;
}

void Line::calculate() {
    glBindVertexArray(_VAO);
    
    // bind 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_points.size(), m_points.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
