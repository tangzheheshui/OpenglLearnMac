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

static const std::string STR_DEPTH_TEXTURE = "depth_texture";
static const std::string STR_TEXTURE_PATH = "/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/res/textures/";
typedef std::shared_ptr<std::vector<unsigned char>> ImageBuffer;

struct Texture {
    std::string name; // 直接用作uniform纹理的名字
    std::string filepath;
    ImageBuffer data;
    int width = 0;
    int height = 0;
    int format = 0;
};

class TextureMng {
public:
    static TextureMng& getInstance() {
        std::call_once(_callOnce, [&]() { _instance = new TextureMng; });
        return *_instance;
    }

    void loadTexture(const std::string &filename);
    unsigned int getTexture(const std::string &filename);
    unsigned int getCubeTexture(std::array<std::string, 6> faces);
private:
    TextureMng();
    ~TextureMng() {}
    TextureMng(const TextureMng& mng) = delete;
    TextureMng& operator = (const TextureMng& mng) = delete;
    TextureMng(TextureMng&& mng) = delete;
    TextureMng& operator = (TextureMng&& mng) = delete;
private:
    void createShadowTexture();
    void createSkyTexture();
    unsigned int genTexture(const Texture& tex);
private:
    inline static TextureMng* _instance = nullptr;
    inline static std::once_flag _callOnce;
    std::map<std::string, unsigned int> m_map_tetures;
    std::map<std::string, Texture> m_map_tex_data;
    mutable std::mutex m_mutux;
};

#endif /* image_hpp */
