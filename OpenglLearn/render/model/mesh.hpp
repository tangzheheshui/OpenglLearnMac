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
#include "model_data.h"

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
    Mesh(std::shared_ptr<MeshData> meshData, std::shared_ptr<Materail> matData);
    bool Draw(const glm::mat4 &matModel, bool bDrawShadow);
    void setBoneMat(std::shared_ptr<std::vector<glm::mat4>> mat) { m_pass->m_matBone = mat; }
private:        
    unsigned int _texture;
    std::shared_ptr<RenderPass> m_pass;
}; 

#endif /* mesh_hpp */
