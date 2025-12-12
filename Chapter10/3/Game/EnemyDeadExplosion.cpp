#include "EnemyDeadExplosion.h"
#include<DxLib.h>

constexpr int img_w = 64;
constexpr int img_h = 64;
constexpr int img_center_x = 64/2;
constexpr int img_center_y = 48;
constexpr int one_pict_frame = 8;
constexpr int pict_num = 8;
constexpr float img_scale= 2.0f;


EnemyDeadExplosion::EnemyDeadExplosion(int h, const Position2& pos):
		Effect(pos),
		handle_(h)
{
}

void EnemyDeadExplosion::Update()
{
	if (++frame_ >= one_pict_frame * pict_num) {
		isDead_ = true;
	}
}

void EnemyDeadExplosion::Draw()
{
	if (isDead_)return;
	int idx = (frame_ / one_pict_frame);
	DrawRectRotaGraph2(pos_.x, pos_.y,
						idx * img_w, 0,
						img_w, img_h,
						img_center_x, img_center_y,
						img_scale,
						0.0f,
						handle_,
						true);
}
