//
//  time.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/11.
//

#ifndef time_h
#define time_h

#include <time.h>

class Timer {
public:
    bool is_end(float t);
private:
    
    float _len; // 秒
    
};
#endif /* time_h */
