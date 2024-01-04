//
//  camera.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/2.
//
    
#include "camera.hpp"

const float YAW         = -90.0f;
const float PITCH       =  0.0f;

Camera::Camera() 
: _yaw(YAW)
, _pitch(PITCH)
{
    
}

void Camera::setPosition(glm::vec3 pos) {
    _position = pos;
}

void Camera::setUp(glm::vec3 up) {
    _worldUp = up;
}

void Camera::setYaw(float yaw) {
    _yaw = yaw;
}

void Camera::setPitch(float pitch) {
    _pitch = pitch;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::caculate() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldUp));  
    _up    = glm::normalize(glm::cross(_right, _front));
}
