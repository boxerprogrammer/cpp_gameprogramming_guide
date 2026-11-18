#include "CannonBall.h"
#include<DxLib.h>
constexpr float img_scale = 2.0f;
constexpr float collision_radius = 7.0f * img_scale;
constexpr int img_width = 16;
constexpr int img_height = 16;
constexpr int img_start_x = 16;
constexpr int img_start_y = 96;
constexpr int anim_num = 4;
constexpr int anim_interval = 1;

//重力加速度
constexpr float gravity = 0.1f;

CannonBall::CannonBall(int handle, const Position2& pos, const Vector2& vel) :
	Bullet(handle, pos, collision_radius),vel_(vel)
{
}

void CannonBall::Update()
{
	auto& pos = GetPosition();
	//速度と座標を同じタイミングで更新することで、等加速度直線運動を実現している
	//重力はY方向にしか作用しないので、X方向の速度は更新させません
	pos += vel_;//座標更新
	vel_.y += gravity;//速度更新
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
	++frame_;
}

void CannonBall::Draw()
{
	const auto& pos = GetPosition();
	int idx = (frame_ / anim_interval) % anim_num;
	DrawRectRotaGraph(pos.x, pos.y,
		img_start_x + idx * img_width, img_start_y,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);
}
