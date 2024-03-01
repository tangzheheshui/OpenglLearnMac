//
//  aabb.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/1.
//

#ifndef aabb_hpp
#define aabb_hpp

class AABB {
public:
    float getMinX() const { return _minX; }
    float getMinY() const { return _minY; }
    float getMinZ() const { return _minZ; }
    float getMaxX() const { return _maxX; }
    float getMaxY() const { return _maxY; }
    float getMaxZ() const { return _maxZ; }
    
    void setMinX(float x) { _minX = x; }
    void setMinY(float x) { _minY = x; }
    void setMinZ(float x) { _minZ = x; }
    void setMaxX(float x) { _maxX = x; }
    void setMaxY(float x) { _maxY = x; }
    void setMaxZ(float x) { _maxZ = x; }
private:
    float _minX = 0.f;
    float _minY = 0.f;
    float _minZ = 0.f;
    float _maxX = 0.f;
    float _maxY = 0.f;
    float _maxZ = 0.f;
};

#endif /* aabb_hpp */
