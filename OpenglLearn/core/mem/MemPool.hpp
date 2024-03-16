//
//  MemPool.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/13.
//

#ifndef MemPool_hpp
#define MemPool_hpp

#include <memory>
#include <mutex>

#define DEFAULT_BLOCK_SIZE 100

template <size_t T>
class MemPool {
public:
    static void* malloc();
    static void free(void* p);
private:
    struct _MemBlock {
        _MemBlock* _next = nullptr;
        char data[T];
    };
    
    MemPool();
    ~MemPool();
private:
    _MemBlock* _freeBlock;
    inline static size_t _blockSize = T;
    static std::mutex _mutex; 
    size_t _totalBlock = DEFAULT_BLOCK_SIZE;
};

#endif /* MemPool_hpp */
