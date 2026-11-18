#include "NormalBullet.h"
#include<DxLib.h>
constexpr float img_scale = 3.0f;
constexpr float collision_radius = 3.0f * img_scale;
constexpr int anim_interval = 4;
constexpr int anim_img_num = 2;
constexpr int img_width = 16;
constexpr int img_height = 16;

NormalBullet::NormalBullet(int handle, const Position2& pos,const Vector2& vel):
	Bullet(handle,pos,collision_radius)
{
	vel_ = vel;
}

void NormalBullet::Update()
{
	auto& pos = coll_->GetCircle().pos;
	pos += vel_;
	++frame_;
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
}

void NormalBullet::Draw()
{
	auto& pos = coll_->GetCircle().pos;
	int idx = (frame_ / anim_interval) % anim_img_num;
	DrawRectRotaGraph(pos.x, pos.y,//中心座標
						idx * img_width, 0,//切り取り左上
						img_width, img_height,
						img_scale, 0.0f,
						handle_, true);
}
