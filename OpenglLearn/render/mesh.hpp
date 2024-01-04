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
#include "../third/glm/glm.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    //glm::vec3 Tangent;
    //glm::vec3 Bitangent;
};

struct Texture {
    std::shared_ptr<std::vector<char>> data;
};

class Mesh {
public:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
        
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    bool Draw();
private:        
    unsigned int _VAO, _VBO, _EBO;
    void setupMesh();
}; 

#endif /* mesh_hpp */
