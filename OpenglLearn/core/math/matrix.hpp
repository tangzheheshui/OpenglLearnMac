//
//  matrix.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/2/24.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <glm/glm.hpp>

class Matrix {
public:
    Matrix();
    Matrix(float v1, float v2, float v3, float v4, 
           float v5, float v6, float v7, float v8, 
           float v9, float v10, float v11, float v12,
           float v13, float v14, float v15, float v16);
    void identity();
    void translate(float x, float y, float z);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scale(float scale);
    void set(int x, int y, float value);
    const float* buffer() const { return &data[0][0];}
    Matrix operator*(const Matrix &other) const;
    static Matrix toMatrix(const glm::mat4 &mat);
    static glm::mat4 toMatrix(const Matrix &mat);
private:
    float data[4][4]{};
    //inline static const Matrix IDENTITY;
};

#endif /* matrix_hpp */
