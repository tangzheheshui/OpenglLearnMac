//
//  matrix.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/2/24.
//

#include "matrix.hpp"
#include <cmath>
#include <iostream>

Matrix::Matrix() {
    identity();
}

Matrix::Matrix(float v1, float v2, float v3, float v4, 
       float v5, float v6, float v7, float v8, 
       float v9, float v10, float v11, float v12,
       float v13, float v14, float v15, float v16) {
    data[0][0] = v1; data[1][0] = v5; data[2][0] = v9; data[3][0] = v13;
    data[0][1] = v2; data[1][1] = v6; data[2][1] = v10; data[3][1] = v14;
    data[0][2] = v3; data[1][2] = v7; data[2][2] = v11; data[3][2] = v15;
    data[0][3] = v4; data[1][3] = v8; data[2][3] = v12; data[3][3] = v16;
    
}
void Matrix::identity() {
    data[0][0] = 1.f; data[0][1] = 0.f; data[0][2] = 0.f; data[0][3] = 0.f;
    data[1][0] = 0.f; data[1][1] = 1.f; data[1][2] = 0.f; data[1][3] = 0.f;
    data[2][0] = 0.f; data[2][1] = 0.f; data[2][2] = 1.f; data[2][3] = 0.f;
    data[3][0] = 0.f; data[3][1] = 0.f; data[3][2] = 0.f; data[3][3] = 1.f;
}

void Matrix::translate(float x, float y, float z) {
    data[3][0] += x;
    data[3][1] += y;
    data[3][2] += z;
}

void Matrix::rotateX(float angle) {
    angle = degrees_to_radians(angle);
    Matrix matRotate;
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    matRotate.set(1, 1, cos);
    matRotate.set(2, 1, -sin);
    matRotate.set(1, 2, sin);
    matRotate.set(2, 2, cos);
    *this = matRotate * (*this);
}

void Matrix::rotateY(float angle) {
    angle = degrees_to_radians(angle);
    Matrix matRotate;
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    matRotate.set(0, 0, cos);
    matRotate.set(2, 0, sin);
    matRotate.set(0, 2, -sin);
    matRotate.set(2, 2, cos);
    *this = matRotate * (*this);
}

void Matrix::rotateZ(float angle) {
    angle = degrees_to_radians(angle);
    Matrix matRotate;
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    matRotate.set(0, 0, cos);
    matRotate.set(1, 0, -sin);
    matRotate.set(0, 1, sin);
    matRotate.set(1, 1, cos);
    *this = matRotate * (*this);
}

void Matrix::scale(float scale) {
    data[0][0] *= scale;
    data[1][1] *= scale;
    data[2][2] *= scale;
}

void Matrix::set(int x, int y, float value) {
    if (0 <= x && x < 4 && 0 <= y && y < 4) {
        data[x][y] = value;
    }
}

Matrix Matrix::operator*(const Matrix &other) const {
    Matrix result;
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.set(i, j, 0);
            for (int k = 0; k < 4; ++k) {
                result.data[i][j] += data[k][j] * other.data[i][k];
            }
        }
    }

    return result;
}

Matrix Matrix::toMatrix(const glm::mat4 &mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //std::cout << " " << mat[i][j];
        }
        //std::cout << std::endl;
    }
    Matrix matrix(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    return matrix;
}

glm::mat4 Matrix::toMatrix(const Matrix &mat) {
    auto matrix = mat.data;
    glm::mat4 ret = glm::mat4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                              matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                              matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                              matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //std::cout << " " << ret[i][j];
        }
        //std::cout << std::endl;
    }
    return ret;
}

