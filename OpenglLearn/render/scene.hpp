//
//  scene.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef scene_hpp
#define scene_hpp

#include <vector>
#include <glad/glad.h>

class BaseDraw;

class Scene {
private:
    Scene() { createObjs(); }
    void createObjs();
public:
    static Scene& getScene();
    
    void drawShadow();
    void draw();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    GLuint _depthMapFBO = 0;
    GLuint _depthTexture = 0;
};

#endif /* scene_hpp */
