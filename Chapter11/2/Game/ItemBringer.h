#pragma once
#include "Enemy.h"
class GameScene;
class ItemFactory;
//アイテムを落とすやつ
class ItemBringer : public Enemy
{
	Vector2 vel_ = {};
	Vector2 avoidVel_ = {};//回避行動のときの速度

	int avoidFrame_ = 0;

	std::weak_ptr<GameScene> gameScene_;
	std::weak_ptr<ItemFactory> itemFactory_;

	using Update_t = void (ItemBringer::*)();
	Update_t update_;
	//前進
	void ForwardUpdate();
	//壁をよける
	void AvoidUpdate();

public:
	ItemBringer(int handle, const Position2& pos, std::weak_ptr<Player> player, 
		std::shared_ptr<EffectFactory> ef, std::shared_ptr<BulletFactory> bf,
		std::weak_ptr<GameScene> gs,std::weak_ptr<ItemFactory> itemFact);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor> actor)override;
};

