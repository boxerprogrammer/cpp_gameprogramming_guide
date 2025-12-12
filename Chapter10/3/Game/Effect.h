#pragma once
#include "Actor.h"
class Effect :
    public Actor
{
protected:
    bool isDead_ = false;
public:
    Effect(const Position2& pos);
    virtual void Update() = 0;
    virtual void Draw() = 0;
    bool IsDead()const;
    void OnHit(const Actor&)override {}
};

