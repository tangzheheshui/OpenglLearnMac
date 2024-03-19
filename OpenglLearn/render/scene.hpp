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
class ImageRectangle;

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
    std::vector<std::shared_ptr<ImageRectangle>> createGlass();
    void creatBlendTexture();
    void clearBlendTexture();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj_blend;
    GLuint _depthMapFBO = 0;
    inline static GLuint _depthTexture = 0;
    inline static Matrix* _lightVPMatrix = nullptr;
    
    GLuint _fragment_alpha_buffer;
    GLuint _atomic_counter;
    GLuint _texture_blend;
    GLuint _pbo_head_pointer;
};

#endif /* scene_hpp */
