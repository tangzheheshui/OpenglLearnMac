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
#include <glm/glm.hpp>

class BaseDraw;

class Scene {
private:
    Scene();
    void createObjs();
public:
    static Scene& getScene();
    static GLuint GetShadowTexture() { return _depthTexture; }
    static glm::mat4 GetLightVPMatrix();
    void drawShadow();
    void draw();
    void update();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    GLuint _depthMapFBO = 0;
    inline static GLuint _depthTexture = 0;
    inline static glm::mat4 _lightVPMatrix;
};

#endif /* scene_hpp */
