#pragma once
#include "Effect.h"
class EnemyDeadExplosion :
    public Effect
{
private:
    int frame_ = 0;
    int handle_ = -1;
public:
    EnemyDeadExplosion(int h, const Position2& pos);
    void Update()override;
    void Draw()override;
};

