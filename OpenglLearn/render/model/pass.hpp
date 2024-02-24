//
//  pass.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#ifndef pass_hpp
#define pass_hpp

#include <vector>
#include "../third/glm/glm.hpp"
#include <glad/glad.h>
#include "camera.hpp"
#include "shader.hpp"
#include "model_data.h"
#include "core/math/matrix.hpp"

class RenderPass {
public:
    RenderPass(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);
    virtual ~RenderPass() {
        if (_VAO > 0) {
            glDeleteVertexArrays(1, &_VAO);
        }
        
        if (_VBO > 0) {
            glDeleteBuffers(1, &_VBO);
        }
        
        if (_EBO > 0) {
            glDeleteBuffers(1, &_EBO);
        }
    }
    
    bool Draw(const std::vector<Matrix> &matModel, bool bDrawShadow);
protected:
    virtual Shader* getShader(bool shadow) { return nullptr; }
    virtual void setup(const std::vector<Matrix> &matModel) {}
public:
    std::shared_ptr<Materail> m_materail;
    std::shared_ptr<MeshData> m_mesh_data;
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _EBO = 0;
    std::shared_ptr<std::vector<glm::mat4>> m_matBone;
};

#endif /* pass_hpp */
