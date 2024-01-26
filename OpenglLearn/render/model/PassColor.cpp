//
//  PassColor.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassColor.hpp"
#include "../Light.h"

PassColor::PassColor(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData) { 
    m_mesh_data = meshData;
    m_buffer.reserve(meshData->positions.size());
    for (int i = 0; i < meshData->positions.size(); i++) {
        BufferPassColor data;
        data.m_pos = meshData->positions[i];
        data.m_normal = meshData->normals[i];
        m_buffer.push_back(data);
    }
    m_materail = matData;
}

bool PassColor::Draw(const glm::mat4 &matModel, bool bDrawShadow) {
    setup();
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model_Color);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    // 矩阵
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uMatrixM", matModel);
    shader->setMat4("uMatrixVP", mpMatrix);
    
    // 灯光
    auto light = Light::GlobalLight();
    shader->setFloat3("uLight.direction", light.direction.x, light.direction.y, light.direction.z);
    shader->setFloat3("uLight.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
    shader->setFloat3("uLight.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
    shader->setFloat3("uLight.specular", light.specular.x, light.specular.y, light.specular.z);
    
    // 材质
    shader->setFloat3("uMaterail.ambient", m_materail->ambient.r, m_materail->ambient.g, m_materail->ambient.b);
    shader->setFloat3("uMaterail.diffuse", m_materail->diffuse.r, m_materail->diffuse.g, m_materail->diffuse.b);
    shader->setFloat3("uMaterail.specular", m_materail->specular.r, m_materail->specular.g, m_materail->specular.b);
    shader->setFloat("uMaterail.shininess", m_materail->shininess);
    shader->setFloat("uMaterail.shininess_strength", m_materail->shininess_strength);
    
    // 相机位置
    auto cam_pos = Camera::GetCamera().getPossition();
    shader->setFloat3("uCameraPos", cam_pos.x, cam_pos.y, cam_pos.z);
  
    // 绘制网格
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_mesh_data->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    return true;
}

void PassColor::setup() {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(BufferPassColor), m_buffer.data() , GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_data->indices.size() * sizeof(unsigned int), m_mesh_data->indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
