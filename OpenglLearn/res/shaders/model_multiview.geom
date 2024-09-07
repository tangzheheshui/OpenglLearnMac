#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices=12) out;

uniform int uNumViewport;
uniform mat4 uMatrixModel[4];
uniform mat4 uMatrixVP;

in vec3 vs_normal[];
in vec2 vs_coord[];

out VS_OUT
{
    vec3 pFragPos;
    vec3 pNormal; 
    vec2 pTexCoord;
    mat3 pMatNormal;
} vs_out;

void main()
{
    for(int i = 0; i < uNumViewport; ++i) {
        gl_ViewportIndex = i;
        for(int j = 0; j < 3; ++j) {
            vs_out.pTexCoord = vs_coord[j];
            vs_out.pNormal = vs_normal[j];
            vs_out.pFragPos = (uMatrixModel[i] * gl_in[j].gl_Position).xyz;
            vs_out.pMatNormal = mat3(transpose(inverse(uMatrixModel[i])));
            gl_Position = uMatrixVP * uMatrixModel[i] * gl_in[j].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}
