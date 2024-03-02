//
//  input.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/2.
//

#include "input.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "render/camera.hpp"

void InputProcessMng::setFrameBufferSize(int w, int h) {
    _frameWidth = w;
    _frameHeight = h;
}

void InputProcessMng::processMouseLeftKeyUp(double x, double y) {
    std::cout << "Left mouse button pressed at (" << x << ", " << y << ")" << std::endl;
}

void InputProcessMng::processKeyRelease(int key) {
    if (key == GLFW_KEY_C) {
        int width = _frameWidth;
        int height = _frameHeight;
        unsigned char* pixels = new unsigned char[width * height * 3]; // 假设使用 RGB 颜色格式
        glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

        // 写入图像数据到文件
        std::string rootPath = "/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/screen_picture/";
        std::ofstream imageFile(rootPath + "output_image.bmp", std::ios::binary);
        if (imageFile.is_open())
        {
            // BMP 文件头
            char bmpHeader[54] = {0};
            bmpHeader[0] = 'B';
            bmpHeader[1] = 'M';
            int imageSize = width * height * 3;
            int fileSize = imageSize + 54;
            bmpHeader[2] = fileSize & 0xFF;
            bmpHeader[3] = (fileSize >> 8) & 0xFF;
            bmpHeader[4] = (fileSize >> 16) & 0xFF;
            bmpHeader[5] = (fileSize >> 24) & 0xFF;
            bmpHeader[10] = 54;
            
            // 图像信息头
            bmpHeader[14] = 40;
            bmpHeader[18] = width & 0xFF;
            bmpHeader[19] = (width >> 8) & 0xFF;
            bmpHeader[20] = (width >> 16) & 0xFF;
            bmpHeader[21] = (width >> 24) & 0xFF;
            bmpHeader[22] = height & 0xFF;
            bmpHeader[23] = (height >> 8) & 0xFF;
            bmpHeader[24] = (height >> 16) & 0xFF;
            bmpHeader[25] = (height >> 24) & 0xFF;
            bmpHeader[26] = 1;
            bmpHeader[28] = 24;
            
            // 写入文件头
            imageFile.write(bmpHeader, 54);
            
            // 写入图像数据
            imageFile.write(reinterpret_cast<char*>(pixels), imageSize);
            
            // 关闭文件
            imageFile.close();
        }
        else
        {
            std::cerr << "Failed to open image file for writing" << std::endl;
        }
        // 释放内存
        delete[] pixels;
    }
}

void InputProcessMng::processScroll(double xOffset, double yOffset) {
    float fov = Camera::GetCamera().getFov();
    fov += (float)yOffset;
    if (fov < 3.0f)
        fov = 3.0f;
    if (fov > 90.0f)
        fov = 90.0f;
    
    Camera::GetCamera().setFov(fov);
    return;
}

void InputProcessMng::processMouseMove(double x, double y) {
    
}
