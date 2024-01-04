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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    _vertices = vertices;
    _indices = indices;
    _textures = textures;
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model);
    if (!shader) {
        return;
    }

    setupMesh(shader);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool Mesh::Draw() {
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model);
    if (!shader) {
        return false;
    }
    
    shader->use();
    //setupMesh(shader);
    
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr   = 0;
    unsigned int heightNr   = 0;
    
    for (int i = 0; i < _textures.size(); i++) {
        auto tex = _textures[i];
        int texID = 0;
        std::string texUnifromName = tex.name;
        if (!tex.filepath.empty()) {
            texID = Image::TextureFromFile(tex.filepath);
            if(tex.name == "texture_diffuse") {
                texUnifromName += std::to_string(diffuseNr++);
            }
            else if(tex.name == "texture_specular") {
                texUnifromName += std::to_string(specularNr++);
            }
            else if(tex.name == "texture_normal") {
                texUnifromName += std::to_string(normalNr++);
            }
            else if(tex.name == "texture_height") {
                texUnifromName += std::to_string(heightNr++);
            }
        }
        else {
            texID = Image::TextureFromMem((unsigned char*)tex.data->data(), (int)tex.data->size());
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texID);
        shader->setInt(texUnifromName, i);
    }
    
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    shader->setMat4("uMvp", projection * view * model);
    
    // 绘制网格
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return true;
}

void Mesh::setupMesh(Shader* shader) {
    if (!shader) {
        return;
    }
    
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices) * sizeof(Vertex), _vertices.data() , GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
