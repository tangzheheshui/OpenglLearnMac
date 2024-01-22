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

class RenderPass {
public:
    RenderPass() {}
    virtual ~RenderPass() {}
    
    virtual bool Draw() = 0;
};

#endif /* pass_hpp */
