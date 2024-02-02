//
//  model_data.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/23.
//

#ifndef model_data_h
#define model_data_h

#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

#define MAX_BONE_INFLUENCE 4

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
    std::vector<glm::ivec4> boneIDs;
    std::vector<glm::vec4> weights;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    int index_materail = -1;
};

struct Materail {
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 ambient;
    float shininess = 0.f;
    float shininess_strength = 0.f;
};

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat qua;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

struct NodeAnim {
    std::string name;
    //glm::mat4 localTransform; 计算的结果
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    glm::mat4 getMat4(float ratio) {
        glm::mat4 translation = getPosition(ratio);
        glm::mat4 rotation = getRotation(ratio);
        return translation * rotation;
    }
    
    glm::mat4 getPosition(float ratio) {
        int index = ratio * positions.size();
        index = (index == positions.size()) ? (index-1) : index;
        printf("getposition, index = %d, total_size = %d\n", index, positions.size());
        return glm::translate(glm::mat4(1.0f), positions[index].position);
    }
    
    glm::mat4 getRotation(float ratio) {
        int index = ratio * rotations.size();
        index = (index == rotations.size()) ? (index-1) : index;
        auto rotation = glm::normalize(rotations[index].qua);
        return glm::toMat4(rotation);
    }
};

struct Animation {
    std::string name;
    double duration = 0;
    double ticksPerSecond = 0;
    std::vector<NodeAnim> nodeAnims;
    
    NodeAnim* findNodeAnim(const std::string &name) {
        for(auto &node : nodeAnims) {
            if (node.name == name) {
                return &node;
            }
        }
        return nullptr;
    }
};

struct Node {
    std::string name;
    glm::mat4 transformation;
    glm::mat4 matCur = glm::mat4(1);
    std::vector<std::shared_ptr<Node>> child;
};

struct BoneInfo {
    std::string nodeName;
    int index = -1;
    glm::mat4 offset;
};

struct ModelData {
    std::vector<std::shared_ptr<MeshData>> meshs; // 必须有
    std::map<std::string, ImageBuffer> map_image;
    std::vector<std::shared_ptr<Materail>> materails;
    std::vector<std::shared_ptr<Node>> nodes; // 第一个就是根节点
    Animation animation;
    std::map<std::string, BoneInfo> mapBoneInfo;
};

#endif /* model_data_h */
