//
//  input.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/2.
//

#ifndef input_hpp
#define input_hpp

class InputProcessMng {
public:
    static void setFrameBufferSize(int w, int h);
    
    // 左键点击抬起
    static void processMouseLeftKeyUp(double x, double y);
    
    // 键盘按键抬起
    static void processKeyRelease(int key);
    
    // 鼠标滚轮
    static void processScroll(double xOffset, double yOffset);
    
    // 鼠标移动
    static void processMouseMove(double x, double y);
private:
    inline static int _frameWidth = 0;
    inline static int _frameHeight = 0;
};

#endif /* input_hpp */
