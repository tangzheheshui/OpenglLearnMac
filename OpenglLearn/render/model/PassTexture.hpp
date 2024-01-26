//
//  PassTexture.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#ifndef PassTexture_hpp
#define PassTexture_hpp

#include "pass.hpp"

struct BufferPassTexture {
    glm::vec3 m_pos;
    glm::vec3 m_normal;
    glm::vec2 m_coord;
};

class PassTexture : public RenderPass {
public:
    PassTexture(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);
    
    virtual bool Draw(const glm::mat4 &matModel, bool bDrawShadow) override;
private:
    void setup();
    virtual bool setShadowShader(const glm::mat4 &matModel) override;
    virtual bool setShader(const glm::mat4 &matModel) override;
private:
    std::vector<BufferPassTexture> m_buffer;
};

#endif /* PassTexture_hpp */
