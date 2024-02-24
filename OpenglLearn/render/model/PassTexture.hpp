//
//  PassTexture.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#ifndef PassTexture_hpp
#define PassTexture_hpp

#include "pass.hpp"

class PassTexture : public RenderPass {
public:
    using RenderPass::RenderPass;
    
private:
    virtual void setup(const std::vector<Matrix> &matModel) override;
    virtual Shader* getShader(bool shadow) override;
};

#endif /* PassTexture_hpp */
