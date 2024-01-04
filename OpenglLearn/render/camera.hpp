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
    void caculate();
    glm::mat4 GetViewMatrix();
    
private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    // euler Angles
    float _yaw;
    float _pitch;
};

#endif /* camera_hpp */
