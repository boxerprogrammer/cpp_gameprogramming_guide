#include "ReflectLaserShot.h"
#include<DxLib.h>
constexpr float grow_per_frame = 8.0f;
constexpr float hit_radius = 3.0f;
constexpr float max_length = 100.0f;
constexpr float shot_speed = 8.0f;
ReflectLaserShot::ReflectLaserShot(int handle, const Position2& pos, const Vector2& vec):
	Shot(handle,pos,hit_radius)
{
	coll_->SetCapsule(pos, -vec * length_, hit_radius);
	vel_ = vec * shot_speed;
}

void ReflectLaserShot::Update()
{
	auto& pos = GetPosition();
	pos += vel_;
	length_ = std::min(length_+grow_per_frame, max_length);
	coll_->SetCapsule(pos, -vel_.Normalized() * length_, hit_radius);
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
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
