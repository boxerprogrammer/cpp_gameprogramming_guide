#include "ChargeShot.h"
#include<DxLib.h>

constexpr int img_width = 63;
constexpr int img_height = 32;
constexpr float img_scale = 2.0f;
constexpr float hit_radius = 8* img_scale;
constexpr float max_length = 32*img_scale;
constexpr float shot_speed = 8.0f;
constexpr int anim_interval = 5;
constexpr int anim_num = 4;

ChargeShot::ChargeShot(int handle, const Position2& pos):Shot(handle,pos,hit_radius)
{
	vel_ = { shot_speed,0.0f };
	Vector2 vec = { 1.0f,0.0f };
	coll_->SetCapsule(pos, vec * max_length, hit_radius);
}

void ChargeShot::Update()
{
	auto& pos = GetPosition();
	pos += vel_;
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
	++frame_;
}

void ChargeShot::Draw()
{
	auto& pos = GetPosition();
	int idx = (frame_ / anim_interval) % anim_num;
	//画像中心位置をずらす
	DrawRectRotaGraph2(pos.x, pos.y,
		idx * img_width, 0,
		img_width, img_height,
		16,16,
		img_scale, 0.0f,
		handle_, true);
	GetCollision()->Draw();
}

void ChargeShot::OnHit(const std::shared_ptr<Actor> actor)
{
	if (!actor) {//壁に当たったらチャージショットといえども死ぬ
		isDead_ = true;
		return;
	}
	if (actor->GetActorType() == ActorType::enemy) {
		if (--life_ < 0) {
			isDead_ = true;
			return;
		}
	}
}
