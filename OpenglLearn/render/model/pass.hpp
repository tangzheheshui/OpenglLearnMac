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

class RenderPass {
public:
    RenderPass() {}
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
    
    virtual bool Draw(const glm::mat4 &matModel) = 0;
public:
    std::shared_ptr<Materail> m_materail;
    std::shared_ptr<MeshData> m_mesh_data;
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _EBO = 0;
};

#endif /* pass_hpp */
