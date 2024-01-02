//
//  shader_string.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef shader_string_h
#define shader_string_h

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
#endif /* shader_string_h */
