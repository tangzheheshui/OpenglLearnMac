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

class Materail {
public:
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 ambient;
    glm::vec4 emission;    
};

class RenderPass {
public:
    RenderPass() {}
    virtual ~RenderPass() {}
    
    virtual bool Draw() = 0;
    void setMaterail(Materail mat) { m_materail = mat; }
public:
    Materail m_materail;
};

#endif /* pass_hpp */
