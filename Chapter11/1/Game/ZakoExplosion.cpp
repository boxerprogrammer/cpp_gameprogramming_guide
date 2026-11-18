#include "ZakoExplosion.h"
#include<DxLib.h>
constexpr int img_width = 48;
constexpr int img_height = 48;
constexpr int img_num = 7;
constexpr int img_interval = 3;
constexpr float img_scale = 2.0f;

ZakoExplosion::ZakoExplosion(const Position2& pos, int handle):
	Effect(pos,img_interval*img_num,handle)
{
}

void ZakoExplosion::Draw()
{
	int idx = frame_ / img_interval;
	const auto& pos = coll_->GetCircle().pos;
	DrawRectRotaGraph(pos.x, pos.y,
		idx * img_width, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);
}
