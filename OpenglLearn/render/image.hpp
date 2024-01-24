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
class Image {
public:
    Image();
    ~Image();
    bool setPath(const char* filepath);
    bool draw();
    static unsigned int TextureFromFile(const std::string &filename);
    static unsigned int TextureFromMem(unsigned char* buffer, int len);
private:
    void setData();
    unsigned int _texture;
    unsigned int _VAO;
    unsigned int _VBO;
    unsigned int _EBO;
    std::string _filename;
    static std::map<const std::string, unsigned int> s_map_texture_cache;
};

#endif /* image_hpp */
