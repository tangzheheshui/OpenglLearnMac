//
//  PassColor.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassColor.hpp"
#include "../Light.h"
#include "../scene.hpp"

Shader* PassColor::getShader(bool shadow) {
    Shader* pShadow = nullptr;
    if (shadow) {
        pShadow = ShaderCache::GetInstance().GetShader(ShaderType::Shadow_Color);
    } else {
        pShadow = ShaderCache::GetInstance().GetShader(ShaderType::Model_Color);
    }
    return pShadow;
}

void PassColor::setup(const std::vector<Matrix> &matModel) {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    
    bool hasAnim = (m_matBone && !m_matBone->empty());
    
    size_t size1 = m_mesh_data->positions.size() * sizeof(glm::vec3);
    size_t size2 = m_mesh_data->normals.size() * sizeof(glm::vec3);
    size_t size3 = m_mesh_data->boneIDs.size() * sizeof(glm::ivec4);
    size_t size4 = m_mesh_data->weights.size() * sizeof(glm::vec4);
    size_t totalsize = size1 + size2;
    if (hasAnim) {
        totalsize += (size3 + size4);
    }
    size_t size5 = sizeof(Matrix) * matModel.size();
    totalsize += size5;
    
    glBufferData(GL_ARRAY_BUFFER, totalsize, nullptr , GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size1, m_mesh_data->positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1, size2, m_mesh_data->normals.data());
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)size1);
    glEnableVertexAttribArray(1);
    
    int bufferIndex = 2;
    if (hasAnim) {
        glBufferSubData(GL_ARRAY_BUFFER, size1 + size2, size3, m_mesh_data->boneIDs.data());
        glBufferSubData(GL_ARRAY_BUFFER, size1 + size2 + size3, size4, m_mesh_data->weights.data());
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_INT, GL_FALSE, sizeof(glm::ivec4), (void*)(size1 + size2));
        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(size1 + size2 + size3));
        bufferIndex += 2;
    }
    
    glBufferSubData(GL_ARRAY_BUFFER, totalsize - size5, size5, matModel.data());
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(totalsize - size5));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(totalsize - size5 + sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(totalsize - size5 + 2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(totalsize - size5 + 3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_data->indices.size() * sizeof(unsigned int), m_mesh_data->indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}
