//
//  ParticleSystem.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/10.
//

#include "ParticleSystem.hpp"

struct DrawData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> coords;
    std::vector<glm::vec4> colors;
};

void ParticleSystem::update() {
    // 更新系统时间，是否结束
    
    // 更新发射器
    for (auto emitter : m_list_emitter) {
        PARTICLE_LIST list = emitter->update();
        m_list_alive.insert(m_list_alive.end(), list.begin(), list.end());
    }
    
    // 更新粒子
    for (auto particle : m_list_alive) {
        particle->update();
        
        if (!particle->is_alive()) {
            m_list_death.insert(m_list_death.end(), particle);
        }
    }
}

void ParticleSystem::draw() {
    std::map<std::string, DrawData> map_draw_data;
    for (auto particle : m_list_alive) {
        DrawData& data = map_draw_data[particle->getTextureName()];
        particle->getDrawData(data.positions, data.coords, data.colors);
    }
    
    // 按纹理 绘制
    auto iter = map_draw_data.begin();
    for (; iter != map_draw_data.end(); iter++) {
        // 
    }
}

bool ParticleSystem::is_end() const {
    return false;
}
