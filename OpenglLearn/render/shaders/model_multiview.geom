#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices=12) out;

uniform int uNumViewport;
uniform mat4 uMatrixModel[4];
uniform mat4 uMatrixVP;

in vec3 vs_normal[];
in vec2 vs_coord[];

out vec2 pTexCoord;
out vec3 pNormal;
out vec3 pFragPos;
out mat3 pMatNormal;

void main()
{
    for(int i = 0; i < uNumViewport; ++i) {
        gl_ViewportIndex = i;
        for(int j = 0; j < 3; ++j) {
            pTexCoord = vs_coord[j];
            pNormal = vs_normal[j];
            pFragPos = (uMatrixModel[i] * gl_in[j].gl_Position).xyz;
            pMatNormal = mat3(transpose(inverse(uMatrixModel[i])));
            gl_Position = uMatrixVP * uMatrixModel[i] * gl_in[j].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}
