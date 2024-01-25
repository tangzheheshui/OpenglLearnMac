#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uMatrixMVP;

void main()
{
    gl_Position = uMatrixMVP * vec4(aPos, 1.0);
}
