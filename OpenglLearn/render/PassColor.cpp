//
//  PassColor.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassColor.hpp"

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

bool PassColor::Draw() {
    setup();
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model_Color);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    glm::mat4 model         = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();

    shader->setMat4("uMvp", mpMatrix * model);
    shader->setFloat4("u_diffuse", m_materail->diffuse.r, m_materail->diffuse.g, m_materail->diffuse.b, m_materail->diffuse.a);
    shader->setFloat4("u_specular", m_materail->specular.r, m_materail->specular.g, m_materail->specular.b, m_materail->specular.a);
    shader->setFloat4("u_ambient", m_materail->ambient.r, m_materail->ambient.g, m_materail->ambient.b, m_materail->ambient.a);
    shader->setFloat4("u_emission", m_materail->emission.r, m_materail->emission.g, m_materail->emission.b, m_materail->emission.a);
  
    // 绘制网格
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_mesh_data->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    return true;
}

void PassColor::setup() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    
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
