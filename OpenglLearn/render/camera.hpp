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

class Camera {
public:
    Camera();
    void setPosition(glm::vec3 pos);
    void setUp(glm::vec3 up);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setFov(float fov);
    
    float getUp();
    float getYaw() { return _yaw; }
    float getPitch() { return _pitch; }
    float getFov() { return _fov; }
    glm::vec3 getPossition() { return _position; }
    
    void caculate();
    glm::mat4 GetVPMatrix();
    static Camera& GetCamera();
private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    // euler Angles
    float _yaw;
    float _pitch;
    float _fov;
    bool _needCal = true;
};

#endif /* camera_hpp */
