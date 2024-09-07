#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentLighDirection;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} vs_out;

struct Light {
    vec4 position;
    vec3 direction;
    float cosTheta;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light uLight;
uniform vec3 uCameraPos;

uniform mat4 uModel;
uniform mat4 uVP;
uniform mat4 uLightSpaceMatrix;

void main()
{
    gl_Position = uVP * uModel * vec4(aPos, 1.0);
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLightSpace = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
    
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * cross(T, B));    
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * uLight.position.xyz;
    vs_out.TangentLighDirection = normalize(TBN * uLight.direction);
    vs_out.TangentViewPos  = TBN * uCameraPos;
    vs_out.TangentFragPos  = TBN * vec3(uModel * vec4(aPos, 1.0));
}
