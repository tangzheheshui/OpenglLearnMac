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
    PassTexture(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);
    
    virtual bool Draw(const std::vector<glm::mat4> &matModel, bool bDrawShadow) override;
private:
    void setup(const std::vector<glm::mat4> &matModel);
    virtual bool setShadowShader() override;
    virtual bool setShader() override;
};

#endif /* PassTexture_hpp */
