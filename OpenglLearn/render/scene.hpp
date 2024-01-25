//
//  scene.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef scene_hpp
#define scene_hpp

#include "shape/Line.hpp"

class Scene {
private:
    Scene() { calculate(); }
    void calculate();
public:
    static Scene& getScene();
    void draw();
private:
    Line m_line_x;
    Line m_line_y;
    Line m_line_z;
};

#endif /* scene_hpp */
