#include "ReflectLaserShot.h"
#include"../Scene/GameScene.h"
#include"ShotFactory.h"
#include"Stage.h"
#include<DxLib.h>
#include"Collision.h"
constexpr float grow_per_frame = 8.0f;
constexpr float hit_radius = 3.0f;
constexpr float max_length = 100.0f;
constexpr float shot_speed = 8.0f;
void ReflectLaserShot::NormalUpdate()
{
	auto& pos = GetPosition();
	pos += vel_;
	length_ = std::min(length_+grow_per_frame, max_length);
	coll_->SetCapsule(pos, -vel_.Normalized() * length_, hit_radius);
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
}
void ReflectLaserShot::ReflectUpdate()
{
	if (length_ <= 0.0f) {
		isDead_ = true;
		return;
	}
	length_ -= grow_per_frame;
	coll_->SetCapsule(GetPosition(), -vel_.Normalized() * length_, hit_radius);	
}
ReflectLaserShot::ReflectLaserShot(int handle, const Position2& pos, const Vector2& vec,std::weak_ptr<GameScene> gs):
	Shot(handle,pos,hit_radius),
	gameScene_(gs)
{
	coll_->SetCapsule(pos, -vec * length_, hit_radius);
	vel_ = vec * shot_speed;
}

void ReflectLaserShot::Update()
{
	(this->*update_)();
}

void ReflectLaserShot::Draw()
{
	auto& cap = GetCollision()->GetCapsule();
	const auto& posA = cap.pos;
	const auto posB = posA + cap.vec;
	DrawLineAA(posA.x, posA.y, posB.x, posB.y, 0x88ff88, hit_radius);
}

void ReflectLaserShot::OnHit(std::shared_ptr<Actor> actor)
{
	if (!actor) {
		return;
	}
	if (actor->GetActorType() == ActorType::enemy) {
		--life_;
	}
	if (life_ < 0) {
		isDead_=true;
	}
}
void ReflectLaserShot::OnHit(std::shared_ptr<Collision> collision)
{
	if (update_ == &ReflectLaserShot::ReflectUpdate) {
		return;
	}
	if (!collision) {
		return;
	}
	if (collision->GetType() == CollisionType::circle) {
		auto& pos = GetPosition();
		//もし速度vel_が円の中心から遠ざかる方向の場合は反応しない
		auto& circle = collision->GetCircle();
		auto toCircle = circle.pos - pos;
		if (Dot(vel_,toCircle) < 0.0f) {
			return;
		}

		auto& capsule = coll_->GetCapsule();
		auto linePos = capsule.pos + capsule.vec;
		auto lineVec = -capsule.vec;
		Vector2 normal;
		float distance = GetIntersectionDistance(linePos,lineVec, circle, normal);
		
		pos = linePos + lineVec.Normalized() * distance;
		capsule.vec = linePos-pos;
		coll_->SetCapsule(pos, capsule.vec, hit_radius);
		update_ = &ReflectLaserShot::ReflectUpdate;
		if (gameScene_.expired()) {
			return;
		}
		gameScene_.lock()->GetShotFactory()->Fire(pos, ShotType::re_reflect_laser, GetReflectionVector(lineVec, normal).Normalized());
	}
	
}