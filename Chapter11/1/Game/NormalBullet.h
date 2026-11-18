#pragma once
#include"Bullet.h"

/// <summary>
/// 通常弾
/// </summary>
class NormalBullet : public Bullet {
private:
	int frame_ = 0;//アニメーション用
public:
	NormalBullet(int handle, const Position2& pos,const Vector2& vel);
	void Update()override;
	void Draw()override;
};