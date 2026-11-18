#pragma once
#include"../Geometry.h"
#include<memory>
class Actor;//持ち主
class Collision
{
private:
	std::weak_ptr<Actor> owner_;
	Circle circle_;
public:
	Collision(const Position2& pos, float radius);
	void SetOwner(std::shared_ptr<Actor> owner);
	std::shared_ptr<Actor> GetOwner();
	Circle& GetCircle();
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
					const std::shared_ptr<Collision> colB);
/// <summary>
/// 座標が画面外に出てたらtrueにする(ただしマージン付き)
/// </summary>
/// <param name="pos">現在の座標</param>
/// <returns>画面外に出てたらtrue,画面内ならfalse</returns>
bool IsOutScreen(const Position2& pos);
