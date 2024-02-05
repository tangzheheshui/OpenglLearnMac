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
    
    virtual bool Draw(const std::vector<glm::mat4> &matModel, bool bDrawShadow) override;
    virtual bool setShadowShader() override;
    virtual bool setShader() override;
private:
    void setup(const std::vector<glm::mat4> &matModel);
};

#endif /* PassColor_hpp */
