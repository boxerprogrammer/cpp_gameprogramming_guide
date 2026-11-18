#pragma once
#include "Bullet.h"
//放物線を描く弾
class CannonBall : public Bullet
{
private:
	Vector2 vel_ = {};
	int frame_ = 0;
public:
	CannonBall(int handle, const Position2& pos, const Vector2& vel);
	void Update()override;
	void Draw()override;
};

