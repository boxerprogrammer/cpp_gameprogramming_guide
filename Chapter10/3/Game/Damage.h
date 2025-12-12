#pragma once
#include "Effect.h"
class Damage :
    public Effect
{
private:
    int frame_ = 0;
    int handle_ = -1;
public:
    Damage(int h,const Position2& pos);
    void Update()override;
    void Draw()override;
};

