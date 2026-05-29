#ifndef ENEMY_HEALTH_BAR_HPP
#define ENEMY_HEALTH_BAR_HPP

#include "Enemy.hpp"
#include "GlyphObject.hpp"

class EnemyHealthBar : public GlyphObject {
public:
    EnemyHealthBar();

    void Update(const Enemy &enemy);

private:
    void SetHealthStep(int step);

    int m_LastHealthStep = -1;
};

#endif
