#pragma once
#include "Actor.h"
class Player :
    public Actor
{
private:
    int frame_=0;
public:
    Player(const Position2& pos);
    ~Player();
    void Update()override;
    void Draw()override;
    bool IsDead() const override;
    virtual void OnHit(const Actor* targetAct)override;
    ActorType GetType()const override;
};

