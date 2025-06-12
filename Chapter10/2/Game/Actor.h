#pragma once

#include"Geometry.h"
#include<memory>

enum class ActorType {
	none,
	player,
	item,
	enemy,
	rock,
	ball
};

class Collider;
/// <summary>
/// ゲーム中に配置可能なオブジェクトの基底クラス
/// </summary>
class Actor
{
protected:
	Position2 pos_;//座標
	std::shared_ptr<Collider> coll_;//コリジョン
	ActorType type_ = ActorType::none;
public:
	Actor(const Position2& pos);
	virtual ~Actor() {};
	virtual void OnHit(const Actor* targetAct) {}
	virtual void Update()=0;
	virtual void Draw() = 0;
	virtual bool IsDead()const = 0;
	virtual ActorType GetType()const = 0;
	const Position2& GetPos()const;
	void Move(const Vector2& vec);
	std::shared_ptr<Collider> GetCollision();
};

