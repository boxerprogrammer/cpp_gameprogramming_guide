#pragma once
#include "Actor.h"
//プレイヤーの弾
class Shot : public Actor
{
	int handle_;
	Vector2 vel_;//弾の速度
	int frame_ = 0;
public:
	Shot();
	void Init();
	void Update()override;
	void Draw()override;
	//ショットが起動される
	void Fire(const Position2& pos);
	void OnHit(std::shared_ptr<Actor>)override;
};

