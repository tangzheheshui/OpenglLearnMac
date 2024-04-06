//
//  BaseDraw.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef BaseDraw_h
#define BaseDraw_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class BaseDraw {
public:
    BaseDraw() {
        if (_VAO == 0) {
            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);
            glGenBuffers(1, &_EBO);
        }
    }
    ~BaseDraw() {
        if (_VAO > 0) {
            glDeleteVertexArrays(1, &_VAO);
        }
        
        if (_VBO > 0) {
            glDeleteBuffers(1, &_VBO);
        }
        
        if (_EBO > 0) {
            glDeleteBuffers(1, &_EBO);
        }
    }
    virtual bool draw() = 0;
    virtual bool drawShadow() { return false; }
    virtual void update() {}
    virtual bool isClick(const glm::vec3 &worldStart, const glm::vec3 &worldEnd, float &fDeep) { return false; }
public:
    virtual void getDebugPoint(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices) {}
    
    bool getLightOpen() const { return _lightOpen; }
    void setLightOpen(const bool &b) { _lightOpen = b; }
    
    void setAlpha(float alpha) { _alpha = alpha; }
    float getAlpha() { return _alpha; }
protected:
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _EBO = 0;
    bool _has_bind_vbo = false;
private:
    bool _lightOpen = true;
    float _alpha = 1.0f;
};

#endif /* BaseDraw_h */
