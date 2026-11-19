#pragma once
#include"Collision.h"
#include<memory>
enum class ActorType {
	none,//
	player,//プレイヤー
	enemy,//敵
	shot,//プレイヤーの弾
	bullet,//敵の弾
	item,//アイテム
	force//フォース
};
/// <summary>
/// ゲームシーンに配置可能な基底クラス
/// </summary>
/// std::enable_shared_from_thisから継承することで
/// 自分自身のthisのshared_ptrを渡すことができる
class Actor : public std::enable_shared_from_this<Actor>
{
protected:
	std::shared_ptr<Collision> coll_;//自分の当たり判定
	bool isDead_ = false;//死んでるフラグ
	ActorType type_=ActorType::none;
public:
	Actor(const Position2& pos, float radius);
	virtual void Update()=0;//座標等の更新
	virtual void Draw() = 0;//描画
	virtual void OnHit(const std::shared_ptr<Actor> actor) = 0;
	virtual void OnHit(const std::shared_ptr<Collision> collision){};
	ActorType GetActorType()const;
	void SetPosition(const Position2& pos);
	void CollisionSet();
	Position2& GetPosition();
	std::shared_ptr<Collision> GetCollision();
	bool IsDead()const;

};

