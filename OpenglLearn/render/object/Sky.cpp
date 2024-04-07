//
//  Sky.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/4/7.
//

#include "Sky.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/image.hpp"

bool Sky::draw() {
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
    if (!_has_bind_vbo) {
        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * 6, skyboxVertices, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        _has_bind_vbo = true;
    }
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Sky);
    if (!shader) {
        return false;
    }
    shader->use();
    
    if (_textureID == 0) {
        _textureID = TextureMng::getInstance().getCubeTexture(_arrImage);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
    shader->setInt("uTexture", 0);
    
    // 矩阵
    auto mpMatrix = Camera::GetCamera().GetViewMatrix();
    shader->setMat4("uViewMat", mpMatrix);
    mpMatrix = Camera::GetCamera().GetProjectMatrix();
    shader->setMat4("uProjMat", mpMatrix);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    return true;
}

void Sky::setCubeImage(const std::array<std::string, 6> &imgs) {
    _arrImage = imgs;
}
