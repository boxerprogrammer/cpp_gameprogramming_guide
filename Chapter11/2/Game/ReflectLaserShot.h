#pragma once
#include "Shot.h"

class GameScene;

/// <summary>
/// 反射レーザー
/// </summary>
class ReflectLaserShot : public Shot
{
private:
	int life_ = 10;
	float length_ = 0;
	std::weak_ptr<GameScene> gameScene_;
	using Update_t = void (ReflectLaserShot::*)();
	void NormalUpdate();
	void ReflectUpdate();
	Update_t update_ = &ReflectLaserShot::NormalUpdate;
public:
	ReflectLaserShot(int handle, const Position2& pos,const Vector2& vec,std::weak_ptr<GameScene> gs);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor> actor)override;
	void OnHit(std::shared_ptr<Collision> collision)override;
};

