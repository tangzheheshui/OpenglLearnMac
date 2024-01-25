//
//  PassColor.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#ifndef PassColor_hpp
#define PassColor_hpp

#include "pass.hpp"

struct BufferPassColor {
    glm::vec3 m_pos;
    glm::vec3 m_normal;
};

class PassColor : public RenderPass {
public:
    PassColor(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);
    
    virtual bool Draw(const glm::mat4 &matModel) override;
    
private:
    void setup();
private:
    std::vector<BufferPassColor> m_buffer;
};

#endif /* PassColor_hpp */
