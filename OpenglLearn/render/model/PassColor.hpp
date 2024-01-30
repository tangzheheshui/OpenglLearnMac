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
    
    virtual bool Draw(const glm::mat4 &matModel, bool bDrawShadow) override;
    virtual bool setShadowShader(const glm::mat4 &matModel) override;
    virtual bool setShader(const glm::mat4 &matModel) override;
private:
    void setup();
};

#endif /* PassColor_hpp */
