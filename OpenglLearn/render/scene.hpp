//
//  scene.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef scene_hpp
#define scene_hpp

#include <vector>

class BaseDraw;

class Scene {
private:
    Scene() { createObjs(); }
    void createObjs();
public:
    static Scene& getScene();
    
    void beginDrawShadow();
    void endDrawShadow();
    void drawShadow();
    
    void beginDraw();
    void endDraw();
    void draw();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
};

#endif /* scene_hpp */
