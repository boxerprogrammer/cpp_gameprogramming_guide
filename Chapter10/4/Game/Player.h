#pragma once

#include"Actor.h"
#include"../Geometry.h"
#include<memory>
class Input;
class Enemy;
class GameScene;
class Player : public Actor
{
	Circle circle_;
	int handle_;//自分が持ってるグラフィクスハンドル

	int dyingFrame_ = 0;
	int frame_ = 0;

	using UpdateFunc_t = void(Player::*)(Input& input);
	void NormalUpdate(Input& input);
	void JustDieUpdate(Input& input);
	void DyingUpdate(Input& input);
	UpdateFunc_t update_;
	GameScene& gameScene_;
public:
	Player(GameScene& gameScene);
	~Player();
	/// <summary>
	/// 自分の持ってる当たり判定情報を返す
	/// </summary>
	/// <returns>当たり判定円</returns>
	const Circle& GetCollision()const;
	bool IsDead()const { return dyingFrame_ > 0; };
	void Update()override;

	void Draw()override;
	void Update(Input&);
	void OnHit(const Actor& actor)override;
};

