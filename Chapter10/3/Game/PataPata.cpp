#include "PataPata.h"
#include<DxLib.h>
#include<cmath>

constexpr int enemy_cut_w = 32;
constexpr int enemy_cut_h = 16;
constexpr float enemy_scale = 2.5f;
constexpr int one_picture_frame = 8;//1つの絵を表示するフレーム
constexpr int picture_num = 2;//アニメーションに必要な絵の枚数

PataPata::PataPata(int handle, std::shared_ptr<Player> ply,
	std::shared_ptr<BulletFactory> bf,
	std::shared_ptr<EffectFactory> ef,
	const Position2& pos) :
	frame_(0),
	handle_(handle),
	Enemy(ply,bf,ef,pos,13.0f)
{
	if (GetRand(2) % 2 == 0) {
		sign_ = -1.0f;
	}
}

void PataPata::Update()
{
	++frame_;
	pos_.x += sinf(((float)frame_ / 20.0f)*sign_);
	pos_.y += 1.0f;
	circle_.pos = pos_;
}

void PataPata::Draw()
{
	int idx = (frame_ / one_picture_frame) % picture_num;
	//一定時間ごとに絵を切り替えて表示
	DrawRectRotaGraph(pos_.x, pos_.y,
		enemy_cut_w*idx , 0,//16ピクセルずらしてアニメーション
		enemy_cut_w, enemy_cut_h,
		enemy_scale, 0.0f,
		handle_, true);
}

void PataPata::OnHit(const Actor& actor)
{
	if (--life_ <= 0) {
		isDead_ = true;
		OnDead();
	}
}
