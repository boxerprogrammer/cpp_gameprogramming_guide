#include "PlayerExplosion.h"
#include<DxLib.h>
constexpr int img_width = 32;
constexpr int img_height = 32;
constexpr int img_num = 8;
constexpr int img_interval = 5;
constexpr float img_scale = 3.0f;
PlayerExplosion::PlayerExplosion(const Position2& pos, int handle):
	Effect(pos,img_interval*img_num,handle)
{
}

void PlayerExplosion::Draw()
{
	int idx = frame_ / img_interval;
	const auto& pos = coll_->GetCircle().pos;
	DrawRectRotaGraph(pos.x, pos.y,
		idx * img_width, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);
}
