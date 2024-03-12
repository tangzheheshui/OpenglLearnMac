//
//  image.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef image_hpp
#define image_hpp

#include <string>
#include <mutex>
#include <map>
#include "BaseDraw.h"
#include "shader.hpp"

typedef std::shared_ptr<std::vector<unsigned char>> ImageBuffer;

struct Texture {
    std::string name; // 直接用作uniform纹理的名字
    std::string filepath;
    ImageBuffer data;
    int width = 0;
    int height = 0;
    int format = 0;
};

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
    static void loadTexture(const std::string &filename);
private:
    static unsigned int genTexture(const Texture& tex);
private:
    void setData();
    unsigned int _texture = 0;
    ShaderType m_type = ShaderType::Image;
    inline static std::map<const std::string, unsigned int> s_map_texture_cache;
    
    // 多线程缓存
    inline static std::mutex _image_mutex;
    inline static std::map<const std::string, Texture> s_map_image;
};

#endif /* image_hpp */
