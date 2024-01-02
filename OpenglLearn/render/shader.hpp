//
//  shader.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>

class Shader {
public:
    Shader(const char* strVertex, const char* strFragment);
    ~Shader();
    // use
    void use();
    
    // set uniform
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    
private:
    // 程序ID
    unsigned int _ID;
};

#endif /* shader_hpp */
