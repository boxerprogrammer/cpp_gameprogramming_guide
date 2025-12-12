#include "Zako.h"
#include"../Application.h"
#include<DxLib.h>

constexpr int enemy_cut_w = 16;
constexpr int enemy_cut_h = 16;
constexpr float enemy_scale = 2.5f;
constexpr int one_picture_frame = 6;//1つの絵を表示するフレーム
constexpr int picture_num = 2;//アニメーションに必要な絵の枚数


void Zako::ForwardUpdate()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	
	pos_.y += 1.0f;
	circle_.pos = pos_;
	if (pos_.y > wsize.h - 32) {
		update_ = &Zako::BackUpdate;
	}
}

void Zako::BackUpdate()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	auto centerX = wsize.w / 2;


	auto velX = 0.0f;
	if ((centerX - pos_.x) != 0.0f){
		velX = (centerX - pos_.x) / abs(centerX - pos_.x);
	}

	pos_.y -= 2.0f;
	pos_.x += velX * 2.0f;
	circle_.pos = pos_;
}

Zako::Zako(int handle, std::shared_ptr<Player> ply,
	std::shared_ptr<BulletFactory> bf,
	std::shared_ptr<EffectFactory> ef,
	const Position2& pos):
	Enemy(ply, bf,ef, pos, 10.0f),handle_(handle),frame_(0)
{
	update_ = &Zako::ForwardUpdate;
}

void Zako::Update()
{
	++frame_;
	(this->*update_)();

}

void Zako::Draw()
{
	int idx = (frame_ / one_picture_frame) % picture_num;
	//一定時間ごとに絵を切り替えて表示
	DrawRectRotaGraph(pos_.x, pos_.y,
						enemy_cut_w * idx, 0,//16ピクセルずらしてアニメーション
						enemy_cut_w, enemy_cut_h,
						enemy_scale, 0.0f,
						handle_, true);
	circle_.Draw();

}

void Zako::OnHit(const Actor& actor)
{
	//雑魚なので１発当たったら死ぬ
	isDead_ = true;
	OnDead();
}
