//
//  PassTexture.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassTexture.hpp"
#include "image.hpp"
#include "../Light.h"
#include "../scene.hpp"

Shader* PassTexture::getShader(bool shadow) {
    Shader* pShadow = nullptr;
    if (shadow) {
        pShadow = ShaderCache::GetInstance().GetShader(ShaderType::Shadow_Texture);
    } else {
        pShadow = ShaderCache::GetInstance().GetShader(ShaderType::Model_Texture);
        if (m_matBone) {
            pShadow = ShaderCache::GetInstance().GetShader(ShaderType::Model_Texture_Anim);
        }
    }
    return pShadow;
}

void PassTexture::setup(const std::vector<glm::mat4> &matModel) {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    
    bool hasAnim = (m_matBone && !m_matBone->empty());
    
    size_t size1 = m_mesh_data->positions.size() * sizeof(glm::vec3);
    size_t size2 = m_mesh_data->normals.size() * sizeof(glm::vec3);
    size_t size3 = m_mesh_data->coords.size() * sizeof(glm::vec2);
    size_t size4 = m_mesh_data->boneIDs.size() * sizeof(glm::ivec4);
    size_t size5 = m_mesh_data->weights.size() * sizeof(glm::vec4);
    size_t size6 = sizeof(glm::mat4) * matModel.size();
    size_t totalsize = size1 + size2 + size3 + size4 + size5 + size6;
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, totalsize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size1, m_mesh_data->positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1, size2, m_mesh_data->normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1 + size2, size3, m_mesh_data->coords.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1 + size2 + size3 + size4 + size5, size6, matModel.data());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_data->indices.size() * sizeof(unsigned int), m_mesh_data->indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)size1);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(size1 + size2));
    glEnableVertexAttribArray(2);
    
    //
    int bufferIndex = 3;
    if (hasAnim) {
        glBufferSubData(GL_ARRAY_BUFFER, size1 + size2 + size3, size4, m_mesh_data->boneIDs.data());
        glBufferSubData(GL_ARRAY_BUFFER, size1 + size2 + size3 + size4, size5, m_mesh_data->weights.data());
        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(glm::ivec4), (void*)(size1 + size2 + size3));
        
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(size1 + size2 + size3 + size4));
        bufferIndex += 2;
    }
    
    size_t baseOffset = size1 + size2 + size3 + size4 + size5;
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(baseOffset));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(baseOffset + sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(baseOffset + 2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
    
    glVertexAttribPointer(bufferIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(baseOffset + 3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(bufferIndex, 1);
    glEnableVertexAttribArray(bufferIndex++);
}

