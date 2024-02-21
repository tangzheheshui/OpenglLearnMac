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
    PassColor(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);

private:
    virtual bool setShadowShader() override;
    virtual bool setShader() override;
    virtual void setup(const std::vector<glm::mat4> &matModel) override;
private:
    
};

#endif /* PassColor_hpp */
