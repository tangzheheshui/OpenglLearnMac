#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uMatrixVP;

out vec3 vs_normal; 
out vec2 vs_coord;

void main()
{
    vs_normal = aNormal;
    vs_coord = aTexCoord;
    gl_Position = vec4(aPos, 1);
}
