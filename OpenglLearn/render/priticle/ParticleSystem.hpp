//
//  ParticleSystem.hpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/10.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <map>
#include "particle.hpp"
#include "emitter.hpp"
#include "affector.hpp"

using EMITTER_SPTR = std::shared_ptr<Emitter>;
using EMITTER_LIST = std::list<EMITTER_SPTR>;

class ParticleSystem {
public:
    void update();
    void draw();
    bool is_end() const;
private:
    float m_duration;
    float m_start_delay;
    EMITTER_LIST m_list_emitter;
    PARTICLE_LIST m_list_alive;
    PARTICLE_LIST m_list_death;
};

#endif /* ParticleSystem_hpp */
