//
//  Line.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef Line_hpp
#define Line_hpp

#include <glm/glm.hpp>
#include <vector>
#include "../BaseDraw.h"

class Line : public BaseDraw{
public:
    Line() {}
    void setData(const std::vector<glm::vec3> &points, const std::vector<unsigned int> &indices);
    void setColor(const glm::vec3 &color) { m_color = color;}
    void setWidth(float width) { m_fWidth = width; }
    void draw();
private:
    void calculate();
private:
    std::vector<glm::vec3> m_points;
    std::vector<unsigned int> m_indices;
    glm::vec3 m_color;
    float m_fWidth = 1;
};

#endif /* Line_hpp */
