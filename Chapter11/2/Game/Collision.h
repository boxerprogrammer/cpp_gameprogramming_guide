#pragma once
#include"../Geometry.h"
#include<memory>
class Actor;//持ち主
enum class CollisionType {
	circle,//円の当たり判定
	capsule//カプセルの当たり判定
};
class Collision
{
private:
	std::weak_ptr<Actor> owner_;
	Circle circle_;
	Capsule capsule_;
	CollisionType type_=CollisionType::circle;
public:
	Collision(const Position2& pos, float radius);
	Collision(const Position2& pos, const Vector2& vec,float radius);
	void SetOwner(std::shared_ptr<Actor> owner);
	std::shared_ptr<Actor> GetOwner();
	void SetCapsule(const Position2& pos, const Vector2& vec, float radius);
	Circle& GetCircle();
	Capsule& GetCapsule();
	CollisionType GetType()const;
	void Draw();//デバッグ用の表示
};
/// <summary>
/// 当たってたらtrue,外れてたらfalseが返る
/// </summary>
/// <param name="colA">コリジョンA</param>
/// <param name="colB">コリジョンB</param>
/// <returns>true:当たり / false:当たってない</returns>
bool IsHit(const Collision& colA, const Collision& colB);
bool IsHit(const std::shared_ptr < Collision> colA, 
	const std::shared_ptr<Collision> colB, 
	const Vector2& offsetA = {},
	const Vector2& offsetB = {});
/// <summary>
/// めり込みベクトルを返す(前提：当たっている)
/// </summary>
/// <param name="colA">コリジョンA</param>
/// <param name="colB">コリジョンB</param>
/// <returns>めり込み量とその方向ベクトル(Aを押し返すベクトル)</returns>
Vector2 GetOverlap(const std::shared_ptr < Collision> colA,
	const std::shared_ptr<Collision> colB);

/// <summary>
/// 座標が画面外に出てたらtrueにする(ただしマージン付き)
/// </summary>
/// <param name="pos">現在の座標</param>
/// <returns>画面外に出てたらtrue,画面内ならfalse</returns>
bool IsOutScreen(const Position2& pos);

float GetIntersectionDistance(const Vector2& pos, const Vector2& line, const Circle& circle,Vector2& normal);
