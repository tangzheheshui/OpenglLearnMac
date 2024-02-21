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

PassTexture::PassTexture(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData) : RenderPass(meshData, matData) { 
}

void PassTexture::setup(const std::vector<glm::mat4> &matModel) {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }
    
    glBindVertexArray(_VAO);
    
    size_t size1 = m_mesh_data->positions.size() * sizeof(glm::vec3);
    size_t size2 = m_mesh_data->normals.size() * sizeof(glm::vec3);
    size_t size3 = m_mesh_data->coords.size() * sizeof(glm::vec2);
    size_t size4 = sizeof(glm::mat4) * matModel.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, (size1 + size2 + size3 + size4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size1, m_mesh_data->positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1, size2, m_mesh_data->normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1 + size2, size3, m_mesh_data->coords.data());
    glBufferSubData(GL_ARRAY_BUFFER, size1 + size2 + size3, size4, matModel.data());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_data->indices.size() * sizeof(unsigned int), m_mesh_data->indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)size1);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(size1 + size2));
    glEnableVertexAttribArray(2);
    
    int bufferIndex = 3;
    size_t baseOffset = size1 + size2 + size3;
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

bool PassTexture::setShadowShader() {
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Shadow_Texture);
    if (!shader) {
        return false;
    }
    shader->use();
    
    glm::mat4 lightSpaceMatrix = Scene::GetLightVPMatrix();
    shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    return true;
}

bool PassTexture::setShader() {
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model_Texture);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    bool has_normal_tex = false;
    bool has_specular_tex = false;
    
    for (int i = 0; i < m_materail->textures.size(); i++) {
        auto tex = m_materail->textures[i];
        int texID = 0;
        if (!tex.filepath.empty()) {
            texID = Image::TextureFromFile(tex.filepath);
            if(tex.name == "texture_diffuse0") {
                
            }
            else if(tex.name == "texture_specular0") {
                has_specular_tex = true;
            }
            else if(tex.name == "texture_normal0") {
                if (!m_mesh_data->tangents.empty()) {
                    has_normal_tex = true;
                }
            }
            else if(tex.name == "texture_height0") {
            }
        }
        else {
            texID = Image::TextureFromMem((unsigned char*)tex.data->data(), (int)tex.data->size());
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texID);
        shader->setInt(tex.name, i);
        shader->setBool("uMaterail.has_normal_tex", has_normal_tex);
        shader->setBool("uMaterail.has_specular_tex", has_specular_tex);
    }
    
    // 矩阵
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setMat4("uMatrixVP", mpMatrix);
    
    // 灯光
    auto light = Light::GlobalLight();
    shader->setFloat4("uLight.position", light.position.x, light.position.y, light.position.z, 1);
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
    return true;
}
