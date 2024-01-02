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
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D texture1;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1, texCoord);\n"
    "}\n\0";

#endif /* shader_string_h */
