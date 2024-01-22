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

struct Texture {
    std::string name; // 直接用作uniform纹理的名字
    std::string filepath;
    std::shared_ptr<std::vector<char>> data;
};

class PassTexture : public RenderPass {
public:
    PassTexture(const std::vector<BufferPassTexture> &data, std::vector<unsigned int> indexs, const std::vector<Texture> &textures);
    
    virtual bool Draw() override;
    
private:
    void setup();
private:
    unsigned int _VAO, _VBO, _EBO;
    std::vector<BufferPassTexture> m_buffer;
    std::vector<unsigned int> m_indexs;
    std::vector<Texture> _textures;
};

#endif /* PassTexture_hpp */
