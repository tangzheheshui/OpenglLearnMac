//
//  shader.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>
#include <map>
#include <glm/mat4x4.hpp>

class Shader {
public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    ~Shader();
    // use
    void use();
    
    // set uniform
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    
private:
    // 程序ID
    unsigned int _ID;
};

enum class ShaderType {
    Invalid = -1,
    Image,
    Model,
};

class ShaderCache {
public:
    static ShaderCache& GetInstance();
    Shader* GetShader(ShaderType type);
private:
    ShaderCache();
private:
    std::map<ShaderType, Shader*> m_map_shader;
};

#endif /* shader_hpp */
