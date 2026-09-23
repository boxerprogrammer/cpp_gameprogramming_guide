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

	//持ち主が死んでいたら弾も削除
	if (owner_.expired()) {
		isDead_ = true;
		return;
	}

	//画面外に出たら弾を削除
	if ((circle_.pos.x < -32&&vel_.x < 0) || (circle_.pos.x > wsize.w + 32&&vel_.x > 0) || 
		(circle_.pos.y < -32&&vel_.y < 0) || (circle_.pos.y > wsize.h + 32&&vel_.y > 0)) {//弾は画面外に出たら死ぬ
		isDead_ = true;
	}
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
