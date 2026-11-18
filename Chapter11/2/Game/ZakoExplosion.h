#pragma once
#include"Effect.h"
class ZakoExplosion : public Effect
{
public:
	ZakoExplosion(const Position2& pos, int handle);
	void Draw()override;
};

