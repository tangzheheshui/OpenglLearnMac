//
//  MemPool.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/13.
//

#include "MemPool.hpp"

template <size_t T>
void* MemPool<T>::malloc() {
    std::lock_guard<std::mutex> guard(_mutex);
}

template <size_t T>
void MemPool<T>::free(void* p) {
    std::lock_guard<std::mutex> guard(_mutex);
}

template <size_t T>
MemPool<T>::MemPool() {
    
}

template <size_t T>
MemPool<T>::~MemPool() {
    
}
