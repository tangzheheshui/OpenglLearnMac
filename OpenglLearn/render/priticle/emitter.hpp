//
//  emitter.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/10.
//

#ifndef emitter_hpp
#define emitter_hpp

#include <list>
#include "node.hpp"
#include "particle.hpp"

using PARTICLE_SPTR = std::shared_ptr<Particle>;
using PARTICLE_LIST = std::list<PARTICLE_SPTR>;

class Emitter : public Node {
public:
    bool isEnable();
    PARTICLE_LIST update();
};

#endif /* emitter_hpp */
