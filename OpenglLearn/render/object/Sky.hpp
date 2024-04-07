//
//  Sky.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/4/7.
//

#ifndef Sky_hpp
#define Sky_hpp

#include "../BaseDraw.h"

class Sky : public BaseDraw {
public:
    Sky() {}
    ~Sky() {}
    virtual bool draw() override;
    void setCubeImage(const std::array<std::string, 6>& imgs);
private:
    std::array<std::string, 6> _arrImage;
    unsigned int _textureID = 0;
};

#endif /* Sky_hpp */
