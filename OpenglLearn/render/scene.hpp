//
//  scene.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef scene_hpp
#define scene_hpp

#include <filesystem>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "core/math/matrix.hpp"

namespace fs = std::filesystem;

class BaseDraw;
class Line;
class Shader;

class Scene {
private:
    Scene();
    void createObjs();
    void loadTexture(const fs::path& dirPath);
public:
    static Scene& getScene();
    static GLuint GetShadowTexture() { return _depthTexture; }
    static Matrix GetLightVPMatrix();
    void drawShadow();
    void draw();
    void update();
    void setLightUniform(Shader* shader);
    void processMouseClick(double x, double y);
private:
    std::shared_ptr<Line> getTestLine();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    GLuint _depthMapFBO = 0;
    inline static GLuint _depthTexture = 0;
    inline static Matrix* _lightVPMatrix = nullptr;
};

#endif /* scene_hpp */
