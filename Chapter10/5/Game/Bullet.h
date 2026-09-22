#pragma once
#include "Actor.h"
#include<memory>
/// <summary>
/// 弾オブジェクト(仮)
/// </summary>
class Bullet : public Actor
{
private:
	std::weak_ptr<Actor> owner_;//弾の所有者(誰が撃ったか)
	Circle circle_ = { {},3.0f };//弾の当たり判定
	Vector2 vel_ = {};//弾の速度
	bool isDead_ = true;//弾無効フラグ
	int frame_ = 0;//弾のフレーム数
	int handle_ = -1;//弾の画像ハンドル
public:
	Bullet() = default;
	Bullet(std::weak_ptr<Actor> owner, int handle,const Position2& pos, const Vector2& vel);
	const Circle& GetCircle() const { return circle_; }
	virtual bool IsDead()const;
	virtual void Update();
	virtual void Draw();
	virtual void OnHit(const Actor& actor);
};

