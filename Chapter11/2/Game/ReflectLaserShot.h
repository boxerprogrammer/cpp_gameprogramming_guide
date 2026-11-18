#pragma once
#include "Shot.h"

/// <summary>
/// 反射レーザー
/// </summary>
class ReflectLaserShot : public Shot
{
private:
	int life_ = 10;
	float length_ = 0;
public:
	ReflectLaserShot(int handle, const Position2& pos,const Vector2& vec);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor> actor)override;
};

