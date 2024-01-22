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
    glm::vec4 m_color;
};

class PassColor : public RenderPass {
public:
    PassColor(const std::vector<BufferPassColor> &data, std::vector<unsigned int> indexs);
    
    virtual bool Draw() override;
    
private:
    void setup();
private:
    unsigned int _VAO, _VBO, _EBO;
    std::vector<BufferPassColor> m_buffer;
    std::vector<unsigned int> m_indexs;
};

#endif /* PassColor_hpp */
