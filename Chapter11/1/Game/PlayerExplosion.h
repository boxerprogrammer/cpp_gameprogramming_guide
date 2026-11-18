#pragma once
#include "Effect.h"
class PlayerExplosion :
    public Effect
{
public:
    PlayerExplosion(const Position2& pos, int handle);
    void Draw()override;
};

