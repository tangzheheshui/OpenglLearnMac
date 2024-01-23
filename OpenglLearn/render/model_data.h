//
//  model_data.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/23.
//

#ifndef model_data_h
#define model_data_h

typedef std::shared_ptr<std::vector<char>> ImageBuffer;

struct Texture {
    std::string name; // 直接用作uniform纹理的名字
    std::string filepath;
    std::shared_ptr<std::vector<char>> data;
};

struct MeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> coords;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    int index_materail = -1;
};

struct Materail {
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 ambient;
    glm::vec4 emission;    
};

struct ModelData {
    std::vector<std::shared_ptr<MeshData>> meshs;
    std::map<std::string, ImageBuffer> map_image;
    std::vector<std::shared_ptr<Materail>> materails;
};

#endif /* model_data_h */
