//
//  PassColor.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassColor.hpp"

PassColor::PassColor(const std::vector<BufferPassColor> &data, std::vector<unsigned int> indexs) {
    m_buffer = data;
    m_indexs = indexs;
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
    
    // 绘制网格
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indexs.size(), GL_UNSIGNED_INT, 0);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexs.size() * sizeof(unsigned int), m_indexs.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BufferPassColor), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
