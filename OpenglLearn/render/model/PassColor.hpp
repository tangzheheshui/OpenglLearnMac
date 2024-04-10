//
//  PassColor.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#ifndef PassColor_hpp
#define PassColor_hpp

#include "pass.hpp"

class PassColor : public RenderPass {
public:
    using RenderPass::RenderPass;

private:
    virtual Shader* getShader(uint32_t flags) override;
    virtual void setup(const std::vector<Matrix> &matModel, uint32_t flags) override;
private:
    
};

#endif /* PassColor_hpp */
