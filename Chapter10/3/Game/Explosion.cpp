#include "Explosion.h"
#include<DxLib.h>

constexpr int img_w = 16;
constexpr int img_h = 16;
constexpr float img_scale = 4.0f;
constexpr int one_pict_frame = 4;
constexpr int pict_num = 5;

Explosion::Explosion(int h , const Position2& pos):Effect(pos),
handle_(h)
{
}

void Explosion::Update()
{
	++frame_;
	if (frame_ >= one_pict_frame * pict_num) {
		isDead_ = true;
	}
}

void Explosion::Draw()
{
	if (isDead_) {
		return;
	}
	int idx = (frame_ / one_pict_frame);
	DrawRectRotaGraph(pos_.x, pos_.y,
		img_w * idx, 0,
		img_w, img_h,
		img_scale, 0.0f, handle_,true);
}
