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
        light.position = {-4.0f, 4.0f, -1.0f};
        light.direction = {1, -1, 0};
        light.ambient = {0.5f, 0.5f, 0.5f};
        light.diffuse = {0.5f, 0.5f, 0.5f};
        light.specular = {0.2f, 0.2f, 0.2f};
        return light;
    }
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif /* Light_h */
