//
//  priticle.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/10.
//

#ifndef priticle_hpp
#define priticle_hpp

#include <vector>
#include "node.hpp"

class Particle : public Node {
public:
    void update();
    bool is_alive() const;
    void getDrawData(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &coords, std::vector<glm::vec4> &colors);
    std::string getTextureName() { return m_texture_name; }
private:
    std::string m_texture_name;
};

#endif /* priticle_hpp */
