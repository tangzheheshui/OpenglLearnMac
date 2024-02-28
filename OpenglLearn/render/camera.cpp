//
//  camera.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/2.
//
    
#include "camera.hpp"

const float YAW         = 0.0f;
const float PITCH       =  0.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera::Camera() 
: _yaw(YAW)
, _pitch(PITCH)
, _fov(45)
{
    _position = {0, 0, 0};
}

void Camera::setPosition(glm::vec3 pos) {
    _position = pos;
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

Matrix Camera::GetVPMatrix() {
    caculate();
    
    float r = std::sqrt(_position.x*_position.x + _position.y*_position.y + _position.z*_position.z);
    float y = r * sin(glm::radians(_pitch));
    float x = r * cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));
    float z = r * cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
    
    Matrix lookat = LookAt({x, y, z}, glm::vec3(0), _worldUp);
    Matrix projection = Camera::perspective(_fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
    return projection * lookat;
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

Matrix Camera::LookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
    auto F = glm::normalize(center - eye);
    auto R = glm::normalize(glm::cross(F, up));
    auto U = glm::cross(R, F);
    
    // R*T矩阵
    Matrix matR;
    matR.set(0, 0, R.x);
    matR.set(1, 0, R.y);
    matR.set(2, 0, R.z);
    matR.set(0, 1, U.x);
    matR.set(1, 1, U.y);
    matR.set(2, 1, U.z);
    matR.set(0, 2, -F.x);
    matR.set(1, 2, -F.y);
    matR.set(2, 2, -F.z);
    matR.set(3, 0, -eye.x * R.x - eye.y * R.y - eye.z * R.z);
    matR.set(3, 1, -eye.x * U.x - eye.y * U.y - eye.z * U.z);
    matR.set(3, 2, eye.x * F.x + eye.y * F.y + eye.z * F.z);
    
    return matR;
}

Matrix Camera::ortho(float left, float right, float bottom, float top, float near, float far) {
    Matrix mat;
    // 步骤1:平移矩阵T
    //    1 0 0 -(r+l)/2
    //    0 1 0 -(t+b)/2
    //    0 0 1 -(n+f)/2
    //    0 0 0 1
    
    // 步骤2:缩放矩阵S
    //    2/(r-l) 0       0       0
    //    0       2/(t-b) 0       0
    //    0       0       2/(f-n) 0
    //    0       0       0       1
    
    // 步骤3:S*T后，将n和f，替换成-n和-f
    mat.set(0, 0, 2.0 / (right - left));
    mat.set(1, 1, 2.0 / (top - bottom));
    mat.set(2, 2, 2.0 / (near - far));
    mat.set(3, 0, - (right + left) / (right - left));
    mat.set(3, 1, - (top + bottom) / (top - bottom));
    mat.set(3, 2, (far + near) / (near - far));
    return mat;
}

Matrix Camera::perspective(float fov, float aspect, float near, float far) {
    float halffov = 1 / degrees_to_radians(fov / 2);
    Matrix mat;
    mat.set(0, 0, halffov / aspect);
    mat.set(1, 1, halffov);
    mat.set(2, 2, (near + far) / (near - far));
    mat.set(2, 3, -1);
    mat.set(3, 2, 2 * near * far / (near - far));
    return mat;
}
