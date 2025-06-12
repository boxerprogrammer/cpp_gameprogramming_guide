#pragma once
#include "Actor.h"
class Item :
    public Actor
{
private:
    int frame_ = 0;
    bool isGot_ = false;
public:
    Item(const Position2& pos);
    ~Item();
    void Update()override;
    void Draw()override;
    ActorType GetType()const override;
    bool IsDead()const override;
    virtual void OnHit(const Actor* targetAct)override;
};

