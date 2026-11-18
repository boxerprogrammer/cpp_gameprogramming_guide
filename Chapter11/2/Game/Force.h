#pragma once
#include "Actor.h"
class Player;
class GameScene;
//フォース(プレイヤーの前方もしくは後方でガードする)
class Force : public Actor
{
	int handle_;
	std::weak_ptr<Player> player_;
	std::weak_ptr<GameScene> gameScene_;
	bool isBack_ = false;
	int frame_ = 0;
public:
	Force(int handle,std::weak_ptr<Player> player, std::weak_ptr<GameScene> gs);
	void Update()override;
	void Draw()override;
	void OnHit(const std::shared_ptr<Actor> actor)override;
	void Run();
};

