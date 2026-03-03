#pragma once
#include"../Geometry.h"

enum class ActorType {
	none,//
	player,//
	enemy,
	enemy_bullet,
	player_shot,
};

/// <summary>
/// 配置可能なゲームオブジェクトの基底クラス
/// </summary>
class Actor
{
protected:
	Position2 pos_;
	ActorType actorType_ = ActorType::none;
public:
	/// <summary>
	/// アクター種別を返す
	/// </summary>
	/// <returns>アクター種別</returns>
	ActorType GetActorType()const { return actorType_; }
	virtual void Update()=0;
	virtual void Draw()=0;
	virtual void OnHit(const Actor& actor) = 0;
	virtual ~Actor() {};
	const Position2& GetPos()const;
};

