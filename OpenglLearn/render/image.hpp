//
//  image.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef image_hpp
#define image_hpp

#include <string>
#include <map>
#include "BaseDraw.h"
#include "shader.hpp"

class Image : public BaseDraw {
public:
    Image();
    ~Image();
    bool setPath(const char* filepath);
    void setTextureID(unsigned int tex) { _texture = tex; }
    void setShaderType(ShaderType type) { m_type = type; }
    virtual bool draw() override;
    static unsigned int TextureFromFile(const std::string &filename);
    static unsigned int TextureFromMem(unsigned char* buffer, int len);
    static std::shared_ptr<std::vector<char>> GetTextureDataFromFile(const std::string &filename);
private:
    void setData();
    unsigned int _texture = 0;
    ShaderType m_type = ShaderType::Image;
    inline static std::map<const std::string, unsigned int> s_map_texture_cache;
};

#endif /* image_hpp */
