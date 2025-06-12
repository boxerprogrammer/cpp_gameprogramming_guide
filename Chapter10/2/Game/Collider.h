#pragma once
#include<memory>
enum class CollisionType {
	none,
	circle,
	box,
	capsule,
};

enum class RigidType {
	none,//押し当りなし
	movable,//押せる
	unmovable//押せない
};

class Actor;//持ち主の型

//当たり判定の基底クラス
class Collider
{
protected:
	Actor* owner_;//持ち主の参照
	bool isRigit_ = false;//押し出し
	bool isDead_ = false;//Colliderを無効にする
	RigidType rigidType_ = RigidType::none;
public:
	explicit Collider(Actor* owner);
	RigidType GetRigidType()const;
	bool IsDead()const { return isDead_; }
	void Kill() { isDead_ = true; }
	Actor* GetOwner();
	/// <summary>
	/// 自分の当たり判定タイプ(Box,Circle,Capsule)
	/// </summary>
	/// <returns>当たり判定タイプ</returns>
	virtual CollisionType GetType()const=0;
	/// <summary>
	/// コリジョンの座標情報等を更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 当たった時に発生するイベントとして定義
	/// </summary>
	/// <param name="target">当たった相手のCollider</param>
	virtual void OnHit(std::shared_ptr<Collider> target)=0;
	
};

