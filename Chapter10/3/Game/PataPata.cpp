#include "PataPata.h"
#include<DxLib.h>
#include<cmath>

constexpr int enemy_cut_w = 32;
constexpr int enemy_cut_h = 16;
constexpr float enemy_scale = 2.5f;
constexpr int one_picture_frame = 8;//1つの絵を表示するフレーム
constexpr int picture_num = 2;//アニメーションに必要な絵の枚数
constexpr float move_speed = 4.0f;//敵のスピード
constexpr float back_move_speed = 6.0f;//敵のスピード

void PataPata::WaveForwardUpdate()
{

	pos_ += vel_;
	pos_.x += sinf(((float)frame_ / 20.0f) * sign_);
	circle_.pos = pos_;
	if (pos_.y > 640) {
		update_ = &PataPata::WaveBackUpdate;
		if ((320 - pos_.x) != 0.0f) {
			//左か右かですが、centerXが真ん中なので、
			//pos_xが真ん中より右にいるとき
			//-？/|？|=-1
			//pos_xが真ん中より左にいるとき
			//？/|？|=1
			vel_.x = (320 - pos_.x) / abs(320 - pos_.x);
		};
		vel_.y = -1.0f;
		vel_ = vel_.Normalized() * back_move_speed;
	}
}

void PataPata::WaveBackUpdate()
{
	pos_ += vel_;
	circle_.pos = pos_;
}

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
	float x = 0.0f;
	if (pos.x != 320) {
		x = (float)(320 - pos_.x) / fabsf((320 - pos_.x));
	}
	 
	update_ = &PataPata::WaveForwardUpdate;
	vel_ = { x,move_speed };

}

void PataPata::Update()
{
	++frame_;
	(this->*update_)();
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
