//
//  BaseDraw.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/25.
//

#ifndef BaseDraw_h
#define BaseDraw_h

#include <glad/glad.h>

class BaseDraw {
public:
    BaseDraw() {}
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
protected:
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _EBO = 0;
};

#endif /* BaseDraw_h */
