#include "BombExplosion.h"
#include<DxLib.h>

constexpr int img_w = 72;
constexpr int img_h = 72;
constexpr float img_scale = 8.0f;
constexpr int one_pict_frame = 4;
constexpr int pict_num = 16;

BombExplosion::BombExplosion(int h, const Position2& pos) :Effect(pos), handle_(h)
{
}

void BombExplosion::Update()
{
	++frame_;
	if (frame_ >= one_pict_frame * pict_num) {
		isDead_ = true;
	}
}

void BombExplosion::Draw()
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
