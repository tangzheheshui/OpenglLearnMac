//
//  model.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/3.
//

#ifndef model_hpp
#define model_hpp

#include "mesh.hpp"
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "model_data.h"
#include "../BaseDraw.h"

class Model : public BaseDraw {
public:
    Model();
    void LoadFile(const std::string &path);
    virtual bool draw() override;
    virtual bool drawShadow() override;
    
    // 模型的属性
    void setScale(float scale);
    glm::mat4 getModelMatrix() { return m_matModel; }
private:
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    void processTexture(const aiScene* scene);
    void processMaterail(const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void genMesh();
private:
    glm::mat4 m_matModel = glm::mat4(1.0f);;
    std::map<std::string, Texture> m_map_tempTexture;
    std::string m_filepath;
    std::vector<std::shared_ptr<Mesh>> m_mesh;
    ModelData m_model_data;
};

#endif /* model_hpp */
