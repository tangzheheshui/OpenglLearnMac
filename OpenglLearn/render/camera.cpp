//
//  camera.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/2.
//
    
#include "camera.hpp"

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera::Camera() 
: _yaw(YAW)
, _pitch(PITCH)
, _fov(45)
{
    _position = {0, 0, 3};
    _worldUp = {0, 1, 0};
}

void Camera::setPosition(glm::vec3 pos) {
    _position = pos;
    _needCal = true;
}

void Camera::setUp(glm::vec3 up) {
    _worldUp = up;
    _needCal = true;
}

void Camera::setYaw(float yaw) {
    _yaw = yaw;
    _needCal = true;
}

void Camera::setPitch(float pitch) {
    _pitch = pitch;
    _needCal = true;
}

void Camera::setFov(float fov) {
    _fov = fov;
    _needCal = true;
}

glm::mat4 Camera::GetVPMatrix() {
    caculate();
    
    glm::mat4 view = glm::lookAt(_position, _position + _front, _up);
    glm::mat4 projection = glm::perspective(glm::radians(_fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    return projection * view;
}

void Camera::caculate() {
    if (!_needCal) {
        return;
    }
    _needCal = false;
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    
    // also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldUp));  
    _up    = glm::normalize(glm::cross(_right, _front));
}

Camera& Camera::GetCamera() {
    static Camera cam;
    return cam;
}
