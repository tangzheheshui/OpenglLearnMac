//
//  math.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/3.
//

#ifndef math_hpp
#define math_hpp

#include "glm/glm.hpp"
#include <vector>

class Math {
public:
    static glm::vec3 intersectLinePlane(const glm::vec3& pointOnPlane, const glm::vec3& planeNormal, const glm::vec3& linePoint, const glm::vec3& lineDirection);
    static bool isInsidePolygon(const std::vector<glm::vec3>& polygon, const glm::vec3& point);
};

#endif /* math_hpp */
