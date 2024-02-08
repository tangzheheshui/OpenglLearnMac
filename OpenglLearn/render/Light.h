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
        light.position = {1.0f, 14.0f, .0f};
        light.direction = {1, -1, 0};
        float ambient = 0.5;
        light.ambient = glm::vec3(ambient);
        float diffuse = 0.5;
        light.diffuse = glm::vec3(diffuse);
        float specular = 1.0;
        light.specular = glm::vec3(specular);
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
