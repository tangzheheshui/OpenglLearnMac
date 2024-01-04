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
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    // retrieve the directory path of the filepath
    m_filepath = path.substr(0, path.find_last_of('/') + 1);
    
    processTexture(scene);
    
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
        m_map_image[name] = pData;
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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        
        // positions
        float x = mesh->mVertices[i].x;
        float y = mesh->mVertices[i].y;
        float z = mesh->mVertices[i].z;
        //std::cout << "pos=(" << mesh->mVertices[i].x << ", " << mesh->mVertices[i].y << ", " << mesh->mVertices[i].z << ")" << std::endl;
        vertex.Position = {x, y, z};
        
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            //std::cout << "Normal = (%f, %f, %f)" << vector.x << vector.y << vector.z << std::endl;
        }
        
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            //std::cout << "TexCoords = (%f, %f)" << vec.x << vec.y << std::endl;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
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
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    // return a mesh object created from the extracted mesh data
    return std::make_shared<Mesh>(vertices, indices, textures);
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
        
        std::cout << "model materail tex name = " << filename << std::endl;
        auto iter = m_map_image.find(std::string(str.C_Str()));
        if (iter != m_map_image.end()) { 
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

void Model::draw() {
    for (auto mesh : m_mesh) {
        mesh->Draw();
    }
}
