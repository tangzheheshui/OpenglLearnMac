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
    virtual ~RenderPass() {}
    
    virtual bool Draw(const glm::mat4 &matModel) = 0;
public:
    std::shared_ptr<Materail> m_materail;
    std::shared_ptr<MeshData> m_mesh_data;
};

#endif /* pass_hpp */
