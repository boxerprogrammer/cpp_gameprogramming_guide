#pragma once
#include "Shot.h"
class GameScene;
//ホーミング弾
class MissileShot :
    public Shot
{
private:
	std::weak_ptr<GameScene> gameScene_;
	std::weak_ptr<Actor> target_;//狙う先のアクター
public:
	MissileShot(int handle, const Position2& pos,std::weak_ptr<GameScene> gs);
	void SelectTarget();
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor>)override;
};

