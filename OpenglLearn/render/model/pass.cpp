//
//  pass.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "pass.hpp"

RenderPass::RenderPass(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData) {
    m_mesh_data = meshData;
    m_materail = matData;
}

bool RenderPass::Draw(const std::vector<glm::mat4> &matModel, bool bDrawShadow) {
    bool ret = false;
    if (bDrawShadow) {
        ret = setShadowShader();
    } else {
        ret = setShader();
    }
    
    if (!ret) {
        return ret;
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
