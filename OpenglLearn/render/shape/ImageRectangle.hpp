//
//  ImageRectangle.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef ImageRectangle_hpp
#define ImageRectangle_hpp

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../BaseDraw.h"

class ImageRectangle : public BaseDraw {
public:
    ImageRectangle() {}
    ~ImageRectangle();
    void draw();
    void setPoints(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &pos4);
    void setImagePath(const std::string &file) { m_image_path = file; }
    void setSetp(float w, float h);
    void calculate();
private:
    // 逆时针
    glm::vec3 _pos1;
    glm::vec3 _pos2;
    glm::vec3 _pos3;
    glm::vec3 _pos4;
    std::string m_image_path;
    glm::vec2 m_step;
    
    // 计算出来的顶点和索引
    struct Buffer {
        glm::vec3 pos;
        glm::vec2 coord;
    };
    std::vector<Buffer> m_buffer;
    std::vector<unsigned int> m_indices;
    unsigned int _texture;
};

#endif /* ImageRectangle_hpp */
