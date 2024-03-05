//
//  camera.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/2.
//

#ifndef camera_hpp
#define camera_hpp

#include "../third/glm/glm.hpp"
#include "../third/glm/gtc/matrix_transform.hpp"
#include "core/math/matrix.hpp"

class Camera {
public:
    Camera();
    void setPosition(glm::vec3 pos);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setFov(float fov);
    
    float getYaw() { return _yaw; }
    float getPitch() { return _pitch; }
    float getFov() { return _fov; }
    glm::vec3 getPossition() { return _position; }
    
    void caculate();
    Matrix GetVPMatrix();
    static Camera& GetCamera();
    static Matrix LookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
    static Matrix ortho(float left, float right, float bottom, float top, float near, float far);
    static Matrix perspective(float fov, float aspect, float near, float far);
    static void screenToWorld(const glm::vec2 &screen, glm::vec3 &world);
private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    const glm::vec3 _worldUp = {0, 1, 0};
    // euler Angles
    float _yaw;
    float _pitch;
    float _fov;
    float _near;
    float _far;
    bool _needCal = true;
};

#endif /* camera_hpp */
