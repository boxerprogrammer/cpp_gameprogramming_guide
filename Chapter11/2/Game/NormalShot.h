#pragma once
#include "Shot.h"
//通常ショット
class NormalShot :
    public Shot
{
public:
	NormalShot(int handle,const Position2& pos);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor>)override;
};


