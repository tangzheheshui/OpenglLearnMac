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
    virtual bool draw() override;
    void setPoints(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &pos4);
    void setImagePath(const std::string &diffuseMap, const std::string &normalMap, const std::string &heightMap);
    void setSetp(float w, float h);
    void calculate();
private:
    void getTangent(const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3, const glm::vec2 &uv4, glm::vec3 &tangent1, glm::vec3 &bitangent1, glm::vec3 &tangent2, glm::vec3 &bitangent2);
private:
    // 逆时针
    glm::vec3 _pos1;
    glm::vec3 _pos2;
    glm::vec3 _pos3;
    glm::vec3 _pos4;
    std::string m_image_diffuse;
    std::string m_image_normal;
    std::string m_image_height;
    glm::vec2 m_step;
    
    // 计算出来的顶点和索引
    struct Buffer {
        glm::vec3 pos;
        glm::vec2 coord;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
    std::vector<Buffer> m_buffer;
    unsigned int _texture;
};

#endif /* ImageRectangle_hpp */
