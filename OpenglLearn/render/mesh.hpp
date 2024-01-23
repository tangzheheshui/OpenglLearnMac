//
//  mesh.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/3.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <string>
#include <vector>

#include "../third/assimp/Importer.hpp"
#include "../third/assimp/scene.h"
#include "../third/assimp/postprocess.h"
#include "PassColor.hpp"
#include "PassTexture.hpp"
#include "../third/glm/glm.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec4 color;
    glm::vec2 TexCoords;
    //glm::vec3 Tangent;
    //glm::vec3 Bitangent;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    bool Draw();
    void setMaterail(const Materail &mat) { m_materail = mat; }
    
private:        
    unsigned int _texture;
    std::shared_ptr<RenderPass> m_pass;
    Materail m_materail;
}; 

#endif /* mesh_hpp */
