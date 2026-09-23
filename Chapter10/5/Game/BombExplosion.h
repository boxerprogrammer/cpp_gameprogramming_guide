#pragma once
#include "Effect.h"
/// <summary>
/// ボム爆発エフェクト
/// </summary>
class BombExplosion :public Effect
{
private:
    int frame_ = 0;
    int handle_ = -1;
public:
    BombExplosion(int h,const Position2& pos);
    void Update()override;
    void Draw()override;
};

