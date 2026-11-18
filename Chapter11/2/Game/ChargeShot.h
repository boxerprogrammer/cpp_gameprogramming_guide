#pragma once
#include "Shot.h"
///溜めショット
class ChargeShot :
    public Shot
{
    int life_ = 20;
public:
    ChargeShot(int handle, const Position2& pos);
    void Update()override;
    void Draw()override;
    void OnHit(const std::shared_ptr<Actor> actor)override;
};

