//
//  scene.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#include "scene.hpp"
#include <glm/glm.hpp>
#include <vector>

Scene& Scene::getScene() {
    static Scene instance;
    return instance;
}

void Scene::calculate() {
    std::vector<unsigned int> indices = {0, 1};
    
    float len = 10;
    glm::vec3 pZero = {0, 0, 0};
    glm::vec3 pX = {len, 0, 0};
    glm::vec3 pY = {0, len, 0};
    glm::vec3 pZ = {0, 0, len};
    // 3个轴
    m_line_x.setData({pZero, pX}, indices);
    m_line_y.setData({pZero, pY}, indices);
    m_line_z.setData({pZero, pZ}, indices);
    
    // 颜色
    m_line_x.setColor({1, 0, 0});
    m_line_y.setColor({0, 1, 0});
    m_line_z.setColor({0, 0, 1});
    
    // 宽度
    float width = 5;
    m_line_x.setWidth(width);
}

void Scene::draw() {
    m_line_x.draw();
    m_line_y.draw();
    m_line_z.draw();
}