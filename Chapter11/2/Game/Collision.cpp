#include "Collision.h"
#include"../Application.h"
#include<algorithm>

constexpr int out_screen_margin = 100;


namespace {
	/// <summary>
	/// 二つの円が交差するかどうか
	/// </summary>
	/// <param name="cA">円A</param>
	/// <param name="cB">円B</param>
	/// <returns>true:交差した / false:交差していない</returns>
	bool IsHit(const Circle& cA, const Circle& cB) {
		return (cA.pos - cB.pos).Length() <= cA.radius + cB.radius;
	}
	bool IsHit(const Circle& cA, const Circle& cB,const Vector2& offsetA, const Vector2& offsetB) {
		return (cA.pos+offsetA - cB.pos+offsetB).Length() <= cA.radius + cB.radius;
	}

	/// <summary>
	/// カプセルと円が交差するかどうか
	/// </summary>
	/// <param name="capA">カプセルA</param>
	/// <param name="cB">円B</param>
	/// <returns>true:交差した / false:交差していない</returns>
	bool IsHit(const Capsule& capA, const Circle& cB) {
		//カプセルの端点から、円の中心場でのベクトルを作る
		auto V = cB.pos - capA.pos;
		auto L = capA.vec.Normalized();//下のベクトルを正規化したもの
		L *= Dot(V, L);//射影ベクトルを求める
		auto S = V - L;//垂線ベクトルを求める
		return S.Length() <= capA.radius + cB.radius;
	}
	bool IsHit(const Capsule& capA, const Circle& cB,const Vector2& offsetA,const Vector2& offsetB) {
		//カプセルの端点から、円の中心場でのベクトルを作る
		auto V = cB.pos+offsetB - capA.pos+offsetA;
		auto L = capA.vec;//下のベクトルを正規化したもの
		auto lenL = L.Length();
		//範囲を線分の範囲内にとどめた割合(0～1までしか動かない)
		auto rate = std::clamp(Dot(V, L) / lenL,0.0f,1.0f);
		return (V - L * rate).Length() <= capA.radius + cB.radius;
	}

}

Collision::Collision(const Position2& pos, float radius)
{
	circle_.pos = pos;
	circle_.radius = radius;
}

Collision::Collision(const Position2& pos, const Vector2& vec, float radius):
	capsule_{pos,vec,radius},
	type_(CollisionType::capsule)
{

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

void Collision::SetCapsule(const Position2& pos, const Vector2& vec, float radius)
{
	type_ = CollisionType::capsule;
	capsule_.pos = pos;
	capsule_.vec = vec;
	capsule_.radius = radius;
}

Circle& Collision::GetCircle()
{
	return circle_;
}

Capsule& Collision::GetCapsule()
{
	return capsule_;
}

CollisionType Collision::GetType() const
{
	return type_;
}

void Collision::Draw()
{
	if (type_ == CollisionType::circle) {
		circle_.Draw(0xaaffaa);
	}
	else {
		capsule_.Draw(0xaaffaa);
	}
	
}


bool IsHit(Collision& colA, Collision& colB)
{
	if (colA.GetType() == CollisionType::circle && colB.GetType() == CollisionType::circle) {
		return IsHit(colA.GetCircle(), colB.GetCircle());
	}
	if (colA.GetType() == CollisionType::capsule && colB.GetType() == CollisionType::circle) {
		return IsHit(colA.GetCapsule(), colB.GetCircle());
	}
	return false;
}

bool IsHit(const std::shared_ptr<Collision> colA, const std::shared_ptr<Collision> colB,
	const Vector2& offsetA,const Vector2& offsetB)
{
	//円の当たり判定：中心と中心の距離を測り、半径を足したものよりも小さければ
	//当たっている
	//const auto& cA = colA->GetCircle();
	//const auto& cB = colB->GetCircle();
	//return ((cB.pos+offsetB) - (cA.pos+offsetA)).Length() <= cA.radius + cB.radius;
	if (colA->GetType() == CollisionType::circle && colB->GetType() == CollisionType::circle) {
		return IsHit(colA->GetCircle(), colB->GetCircle(),offsetA,offsetB);
	}
	if (colA->GetType() == CollisionType::capsule && colB->GetType() == CollisionType::circle) {
		return IsHit(colA->GetCapsule(), colB->GetCircle(),offsetA,offsetB);
	}
	return false;
}

Vector2 GetOverlap(const std::shared_ptr<Collision> colA, const std::shared_ptr<Collision> colB)
{
	const auto& cA = colA->GetCircle();
	const auto& cB = colB->GetCircle();
	Vector2 BA = cA.pos - cB.pos;//B→Aのベクトル
	//めり込み量を計算
	float overlap=(cA.radius + cB.radius - BA.Length());
	return BA.Normalized() * overlap;
}

bool IsOutScreen(const Position2& pos)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	return wsize.w + out_screen_margin < pos.x || //右の画面外
			pos.x < 0 - out_screen_margin || //左の画面外
			wsize.h + out_screen_margin < pos.y || //下の画面外
			pos.y < 0 - out_screen_margin; //上の画面外
	
}

float GetIntersectionDistance(const Vector2& pos, const Vector2& line, const Circle& circle, Vector2& normal)
{
	auto vec = circle.pos - pos;
	auto projVec = GetProjectionVector(vec, line);//射影ベクトル
	auto verticalVec = vec - projVec;//垂線ベクトル
	auto verticalLen = verticalVec.Length();
	auto distance = projVec.Length()-sqrtf(circle.radius * circle.radius - verticalLen * verticalLen);

	normal = line.Normalized() * distance - vec;
	normal.Normalize();//円の法線ベクトルを求める
	return distance;
}
