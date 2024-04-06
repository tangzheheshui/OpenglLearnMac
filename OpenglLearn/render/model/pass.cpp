//
//  pass.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "pass.hpp"
#include "../scene.hpp"
#include "../Light.h"
#include "image.hpp"

RenderPass::RenderPass(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData) {
    m_mesh_data = meshData;
    m_materail = matData;
}

bool RenderPass::Draw(const std::vector<Matrix> &matModel, uint32_t flags) {
    bool bDrawShadow = (flags & DrawOption::DRAW_SHADOW);
    auto shader = getShader(bDrawShadow);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
    if (bDrawShadow) {
        auto lightSpaceMatrix = Scene::GetLightVPMatrix();
        shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    } else {
        bool has_normal_tex = false;
        bool has_specular_tex = false;
        
        for (int i = 0; i < m_materail->textures.size(); i++) {
            auto tex = m_materail->textures[i];
            int texID = 0;
            if (!tex.filepath.empty()) {
                texID = TextureMng::getInstance().getTexture(tex.filepath);
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
        bool lightOpen = flags&DrawOption::LIGHT_OPEN;
        auto light = Light::GlobalLight();
        shader->setBool("uLight.is_open", lightOpen);
        Scene scene = Scene::getScene();
        scene.setLightUniform(shader);
        
        // 材质
        shader->setFloat3("uMaterail.ambient", m_materail->ambient.r, m_materail->ambient.g, m_materail->ambient.b);
        shader->setFloat3("uMaterail.diffuse", m_materail->diffuse.r, m_materail->diffuse.g, m_materail->diffuse.b);
        shader->setFloat3("uMaterail.specular", m_materail->specular.r, m_materail->specular.g, m_materail->specular.b);
        shader->setFloat("uMaterail.shininess", m_materail->shininess);
        shader->setFloat("uMaterail.shininess_strength", m_materail->shininess_strength);
        
        // 相机位置
        auto cam_pos = Camera::GetCamera().getPossition();
        shader->setFloat3("uCameraPos", cam_pos.x, cam_pos.y, cam_pos.z);
        
        // boneMat
        if (m_matBone && !m_matBone->empty()) {
            for (int i = 0; i < m_matBone->size(); ++i) {
                auto mat = Matrix::toMatrix(m_matBone->at(i));
                shader->setMat4("uFinalBonesMatrices[" + std::to_string(i) + "]", mat);
            }
        }
    }
    
    setup(matModel);
  
    // 绘制网格
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindVertexArray(_VAO);
    glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)m_mesh_data->indices.size(), GL_UNSIGNED_INT, 0,  (int)matModel.size());
    glBindVertexArray(0);
    
    return true;
}
