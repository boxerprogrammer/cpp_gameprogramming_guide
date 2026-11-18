#include "Collision.h"
#include"../Application.h"

constexpr int out_screen_margin = 100;

Collision::Collision(const Position2& pos, float radius)
{
	circle_.pos = pos;
	circle_.radius = radius;
}

void Collision::SetOwner(std::shared_ptr<Actor> owner)
{
	owner_ = owner;
}

std::shared_ptr<Actor> Collision::GetOwner()
{
	if (owner_.expired()) {
		return nullptr;
	}
	return owner_.lock();
}

Circle& Collision::GetCircle()
{
	return circle_;
}

void Collision::Draw()
{
	circle_.Draw(0xaaffaa);
}

bool IsHit(Collision& colA, Collision& colB)
{
	const auto& cA = colA.GetCircle();
	const auto& cB = colB.GetCircle();
	return (cB.pos - cA.pos).Length() <= cA.radius + cB.radius;
}

bool IsHit(const std::shared_ptr<Collision> colA, const std::shared_ptr<Collision> colB)
{
	//円の当たり判定：中心と中心の距離を測り、半径を足したものよりも小さければ
	//当たっている
	const auto& cA = colA->GetCircle();
	const auto& cB = colB->GetCircle();
	return (cB.pos - cA.pos).Length() <= cA.radius + cB.radius;
}

bool IsOutScreen(const Position2& pos)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	return wsize.w + out_screen_margin < pos.x || //右の画面外
			pos.x < 0 - out_screen_margin || //左の画面外
			wsize.h + out_screen_margin < pos.y || //下の画面外
			pos.y < 0 - out_screen_margin; //上の画面外
	
}
