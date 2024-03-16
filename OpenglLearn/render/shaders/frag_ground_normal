#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentLighDirection;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;

struct Light {
    vec4 position;
    vec3 direction;
    float cosTheta;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light uLight;
uniform sampler2D uTexture;
uniform sampler2D uTextureShadowMap;
uniform sampler2D uTextureNormal;
uniform sampler2D uTextureHeight;
uniform int uHasNormal;
uniform int uHasHeight;
uniform float uAlpha;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    if (currentDepth > 1.f) {
        return 0.f;
    }
    float shadow = 0.f;
    
    vec2 texelSize = 1.0 / textureSize(uTextureShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uTextureShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(uTextureHeight, texCoords).r;    
    vec2 p = vec2(2.0, 0.1) * viewDir.xy * height;
    return texCoords + p;    
}

void main()
{
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 dirLight = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float cos = dot(-dirLight, fs_in.TangentLighDirection);
    
    vec2 texCoord = fs_in.TexCoord;
    if (uHasHeight > 0.f) {
        texCoord = ParallaxMapping(fs_in.TexCoord, viewDir);
    }
    vec3 color = texture(uTexture, texCoord).xyz;
    
    vec3 normal = vec3(0, 0, 1);
    if (uHasNormal > 0.f) {
        normal = texture(uTextureNormal, fs_in.TexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0); 
    }
    
    // ambient
    vec3 result = uLight.ambient * color;
    
    if (cos > uLight.cosTheta) {
        // diffuse
        float diff = max(dot(dirLight, normal), 0.0);
        result += uLight.diffuse * color * diff;
    
        // specular
        vec3 halfwayDir = normalize(dirLight + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        result += uLight.specular * spec;
    }
    
    // shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace); 
    float dark = 1 - shadow * 0.7;
    
    FragColor = vec4(dark * result, uAlpha);
}
