#pragma once
#include "Actor.h"
class Enemy :
    public Actor
{
private:
    int frame_ = 0;
public:
    Enemy(const Position2& pos);
    ~Enemy();
    void Update()override;
    void Draw()override;
    bool IsDead() const override;
    virtual void OnHit(const Actor* targetAct)override;
    ActorType GetType()const override;
};

