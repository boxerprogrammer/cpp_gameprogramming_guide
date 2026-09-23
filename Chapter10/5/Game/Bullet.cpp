#include "Bullet.h"
#include"../Application.h"
#include<DxLib.h>
constexpr int img_w = 16;
constexpr int img_h = 16;
constexpr int img_frame = 4;
constexpr int img_num = 4;
constexpr float img_scale = 1.2f;

Bullet::Bullet(std::weak_ptr<Actor> owner, int handle,const Position2& pos, const Vector2& vel): 
	handle_(handle), 
	owner_(owner), 
	circle_(pos, 3.0f), 
	vel_(vel), 
	isDead_(false) {
	pos_ = pos;
	actorType_ = ActorType::enemy_bullet;
}

bool Bullet::IsDead() const
{
	return isDead_;
}

void Bullet::Update()
{
	++frame_;
	const auto& wsize = Application::GetInstance().GetWindowSize();

	pos_ += vel_;
	circle_.pos = pos_;
}

void Bullet::Draw()
{
	int idx = (frame_ / img_frame) % img_num;
	DrawRectRotaGraph(pos_.x, pos_.y,
		img_w * idx, 0,
		img_w, img_h,
		img_scale, 0.0f, handle_, true);

}

void Bullet::OnHit(const Actor & actor)
{
	isDead_ = true;
}
