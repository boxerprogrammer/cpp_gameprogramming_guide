#include "Damage.h"
#include<DxLib.h>

constexpr int img_w = 32;
constexpr int img_h = 32;
constexpr float img_scale = 2.0f;
constexpr int one_pict_frame = 3;
constexpr int pict_num = 8;

Damage::Damage(int h , const Position2& pos):Effect(pos),handle_(h)
{
}


void Damage::Update()
{
	++frame_;
	if (frame_ >= one_pict_frame * pict_num) {
		isDead_ = true;
	}
}

void Damage::Draw()
{
	if (isDead_) {
		return;
	}
	int idx = (frame_ / one_pict_frame);
	DrawRectRotaGraph(pos_.x, pos_.y,
		img_w * idx, 0,
		img_w, img_h,
		img_scale, 0.0f, handle_, true);
}
