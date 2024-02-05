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
#include <time.h>

class AssimpGLMHelpers
{
public:

    static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }

    static inline glm::vec3 GetGLMVec(const aiVector3D& vec) 
    { 
        return glm::vec3(vec.x, vec.y, vec.z); 
    }
    
    static inline glm::vec2 GetGLMVec2(const aiVector2D& vec) 
    { 
        return glm::vec2(vec.x, vec.y); 
    }

    static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
    {
        return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
    }
};

class Model : public BaseDraw {
public:
    Model();
    void LoadFile(const std::string &path);
    virtual bool draw() override;
    virtual bool drawShadow() override;
    virtual void update() override;
    
    // 模型的属性
    void setCount(int count);
    void setScale(int index, float scale);
    void setPosition(int index, const glm::vec3 &pos);
private:
    void processNode(aiNode* node, const aiScene* scene, std::shared_ptr<Node> nodeParent);
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    void processBoneWeightForVertices(aiMesh *mesh, std::shared_ptr<MeshData> meshData);
    void processTexture(const aiScene* scene);
    void processMaterail(const aiScene* scene);
    void processAnimation(const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void genMesh();
    void updateNode(Node* node, Node* nodeParent);
private:
    std::vector<glm::mat4> m_vec_modelMat;
    std::map<std::string, Texture> m_map_tempTexture;
    std::string m_filepath;
    std::vector<std::shared_ptr<Mesh>> m_mesh;
    ModelData m_model_data;
    std::shared_ptr<std::vector<glm::mat4>> m_FinalBoneMatrices;
    clock_t m_clock;
    float m_anim_ratio = 0.f;
    int m_BoneCounter = 0;
};

#endif /* model_hpp */
