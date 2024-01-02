//
//  image.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/1/1.
//

#ifndef image_hpp
#define image_hpp

class Shader;
class Image {
public:
    Image();
    ~Image();
    bool setPath(const char* filepath);
    bool draw();
private:
    unsigned int _texture;
    unsigned int _VAO;
    unsigned int _VBO;
    unsigned int _EBO;
    Shader* _shader;
};

#endif /* image_hpp */
