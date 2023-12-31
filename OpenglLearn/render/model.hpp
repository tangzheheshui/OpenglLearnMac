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

typedef std::shared_ptr<std::vector<char>> ImageBuffer;

class Model {
public:
    Model();
    void LoadFile(const std::string &path);
    void draw();
private:
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    void processTexture(const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
private:
    std::vector<std::shared_ptr<Mesh>> m_mesh;
    std::map<std::string, ImageBuffer> m_map_image;
    std::map<std::string, Texture> m_map_tempTexture;
    std::string m_filepath;
};

#endif /* model_hpp */
