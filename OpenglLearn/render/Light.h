//
//  Light.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/24.
//

#ifndef Light_h
#define Light_h

#include <glm/glm.hpp>

class Light {
public:
    static Light& GlobalLight() {
        static Light light;
        light.direction = {1, -1, 0};
        light.ambient = {0.1f, 0.1f, 0.1f};
        light.diffuse = {0.9f, 0.9f, 0.9f};
        light.specular = {1.0f, 1.0f, 1.0f};
        return light;
    }
public:
    //vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif /* Light_h */
