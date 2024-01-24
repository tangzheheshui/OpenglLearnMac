//
//  model.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/3.
//

#include "model.hpp"
#include "image.hpp"
#include <iostream>
#include <filesystem>

Model::Model() {
}

void Model::LoadFile(const std::string &path) {
    // Load the model file.
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    //import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
    
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    // retrieve the directory path of the filepath
    m_filepath = path.substr(0, path.find_last_of('/') + 1);
    
    processTexture(scene);
    
    // 解析材质
    processMaterail(scene);
    
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processTexture(const aiScene* scene) {
    if (!scene || !scene->HasTextures()) {
        return;
    }
    for (unsigned int i = 0; i < scene->mNumTextures; i++) {
        aiTexture *texture = scene->mTextures[i];
        if (!texture) {
            continue;
        }
        //image_data.name = texture->mFilename.C_Str();
        auto name = std::string("*") + std::to_string(i);
        std::cout << "model data, image name = " << name << std::endl;
        char* buf = (char*)texture->pcData;
        std::shared_ptr<std::vector<char>> pData = std::make_shared<std::vector<char>>();
        pData->assign(buf, buf + texture->mWidth);
        m_model_data.map_image[name] = pData;
    }
}

void Model::processMaterail(const aiScene* scene) {
    if (!scene || !scene->HasMaterials()) {
        return;
    }
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *mat = scene->mMaterials[i];
        if (!mat) {
            continue;
        }
        std::shared_ptr<Materail> tmpMat = std::make_shared<Materail>();
        aiColor4D diffuse;
        aiColor4D specular;
        aiColor4D ambient;
        float shininess;
        float shininess_strength;
        
        if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
            tmpMat->diffuse = {diffuse.r, diffuse.g, diffuse.b, diffuse.a};
        }
           
        if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
            // 防止模型导出时，丢失ambient
            aiColor4D color(0.0, 0.0, 0.0, ambient.a);
            if (ambient == color) {
                tmpMat->ambient = tmpMat->diffuse;
            } 
            else {
                tmpMat->ambient = {ambient.r, ambient.g, ambient.b, ambient.a};
            }
        }
        
        if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular)) {
            tmpMat->specular = {specular.r, specular.g, specular.b, specular.a};
        }
        
        if (AI_SUCCESS == aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess)) {
            tmpMat->shininess = shininess;
        }
        
        if (AI_SUCCESS == aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH, &shininess_strength)) {
            tmpMat->shininess = shininess_strength;
        }
        m_model_data.materails.push_back(tmpMat);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_mesh.push_back(processMesh(mesh, scene));
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();
    int numVertices = mesh->mNumVertices;
    meshData->positions.reserve(numVertices);
    meshData->normals.reserve(numVertices);
    meshData->coords.reserve(numVertices);
  
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // positions
        float x = mesh->mVertices[i].x;
        float y = mesh->mVertices[i].y;
        float z = mesh->mVertices[i].z;
        meshData->positions.push_back({x, y, z});
        
        // normals
        if (mesh->HasNormals())
        {
            x = mesh->mNormals[i].x;
            y = mesh->mNormals[i].y;
            z = mesh->mNormals[i].z;
            meshData->normals.push_back({x, y, z});
        }
        
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            x = mesh->mTextureCoords[0][i].x; 
            y = mesh->mTextureCoords[0][i].y;
            meshData->coords.push_back({x, y});
            
            // tangent
            if (mesh->mTangents) {
                x = mesh->mTangents[i].x;
                y = mesh->mTangents[i].y;
                z = mesh->mTangents[i].z;
            }
            
            //vertex.Tangent = vector;
            
            // bitangent
            if (mesh->mBitangents) {
                x = mesh->mBitangents[i].x;
                y = mesh->mBitangents[i].y;
                z = mesh->mBitangents[i].z;
            }
            
            //vertex.Bitangent = vector;
        }
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            meshData->indices.push_back(face.mIndices[j]);        
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    
    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    meshData->textures.insert(meshData->textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    meshData->textures.insert(meshData->textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    meshData->textures.insert(meshData->textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    meshData->textures.insert(meshData->textures.end(), heightMaps.begin(), heightMaps.end());
    
    // 材质索引
    meshData->index_materail = mesh->mMaterialIndex;
    auto pMesh = std::make_shared<Mesh>(meshData, m_model_data.materails[mesh->mMaterialIndex]);
    return pMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filename = str.C_Str();
        
        Texture texture;
        texture.name = typeName + std::to_string(i);
        
        auto iterCache = m_map_tempTexture.find(filename);
        if (iterCache != m_map_tempTexture.end()) {
            continue; // 是不是需要记录一些东西啊？？
        }
        
        std::cout << "model materail tex name = " << filename << "typeName = " << texture.name << std::endl;
        auto iter = m_model_data.map_image.find(std::string(str.C_Str()));
        if (iter != m_model_data.map_image.end()) { 
            // 从内存中获取
            texture.data = iter->second;
        }
        else {
            size_t pos = filename.find_last_of('\\');
            if (pos == std::string::npos) {
                pos = filename.find_last_of('/');
            }
            auto name = filename.substr(pos+1, filename.length() - pos);
            texture.filepath = m_filepath + name;
        }
        m_map_tempTexture[filename] = texture;
        textures.push_back(texture);
    }
    return textures;
}

void Model::genMesh() {
    
}

void Model::draw() {
    for (auto mesh : m_mesh) {
        mesh->Draw(m_matModel);
    }
}

void Model::setScale(float scale) {
    m_matModel = glm::scale(m_matModel, glm::vec3(scale, scale, scale));
}
