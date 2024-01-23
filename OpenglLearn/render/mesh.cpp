//
//  mesh.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/3.
//

#include "mesh.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "image.hpp"
#include "camera.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    
    if (textures.empty()) {
        std::vector<BufferPassColor> buffer;
        buffer.reserve(vertices.size());
        for (auto v : vertices) {
            BufferPassColor data;
            data.m_pos = v.Position;
            data.m_normal = v.Normal;
            data.m_color = v.color;
            buffer.push_back(data);
        }
        m_pass = std::make_shared<PassColor>(buffer, indices);
    } else {
        std::vector<BufferPassTexture> buffer;
        buffer.reserve(vertices.size());
        for (auto v : vertices) {
            BufferPassTexture data;
            data.m_pos = v.Position;
            data.m_normal = v.Normal;
            data.m_coord = v.TexCoords;
            buffer.push_back(data);
        }
        m_pass = std::make_shared<PassTexture>(buffer, indices, textures);
    }
}

bool Mesh::Draw() {
    m_pass->setMaterail(m_materail);
    if (m_pass) {
         m_pass->Draw();
    }
    
    return true;
}

