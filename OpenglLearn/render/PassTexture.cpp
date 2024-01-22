//
//  PassTexture.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/22.
//

#include "PassTexture.hpp"
#include "image.hpp"

PassTexture::PassTexture(const std::vector<BufferPassTexture> &data, std::vector<unsigned int> indexs, const std::vector<Texture> &textures) {
    m_buffer = data;
    m_indexs = indexs;
    _textures = textures;
}

bool PassTexture::Draw() {
    setup();
    
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Model_Texture);
    if (!shader) {
        return false;
    }
    
    shader->use();
    
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
    
    glm::mat4 model         = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();

    shader->setMat4("uMvp", mpMatrix * model);
    
    // 绘制网格
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indexs.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return true;
}

void PassTexture::setup() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(BufferPassTexture), m_buffer.data() , GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexs.size() * sizeof(unsigned int), m_indexs.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassTexture), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BufferPassTexture), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BufferPassTexture), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
