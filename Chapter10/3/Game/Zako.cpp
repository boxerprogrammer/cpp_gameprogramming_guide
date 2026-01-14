#include "Zako.h"
#include"../Application.h"
#include<DxLib.h>

constexpr int enemy_cut_w = 16;
constexpr int enemy_cut_h = 16;
constexpr float enemy_scale = 2.5f;
constexpr int one_picture_frame = 6;//1つの絵を表示するフレーム
constexpr int picture_num = 2;//アニメーションに必要な絵の枚数
constexpr float move_speed = 4.0f;//敵のスピード
constexpr float back_move_speed = 6.0f;//敵のスピード


//下方向に向かう状態
void Zako::ForwardUpdate()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto centerX = wsize.w / 2;
	pos_ += vel_;
	circle_.pos = pos_;
	//画面最下端に到達したら、上方向に戻る
	if (pos_.y > wsize.h - enemy_cut_h*enemy_scale) {
		update_ = &Zako::BackUpdate;
		if ((centerX - pos_.x) != 0.0f) {
			//左か右かですが、centerXが真ん中なので、
			//pos_xが真ん中より右にいるとき
			//-？/|？|=-1
			//pos_xが真ん中より左にいるとき
			//？/|？|=1
			vel_.x = (centerX - pos_.x) / abs(centerX - pos_.x);
		};
		vel_.y = -1.0f;
		vel_ = vel_.Normalized() * back_move_speed;
	}
}

void Zako::BackUpdate()
{
	pos_ += vel_;
	circle_.pos = pos_;
}

Zako::Zako(int handle, std::shared_ptr<Player> ply,
	std::shared_ptr<BulletFactory> bf,
	std::shared_ptr<EffectFactory> ef,
	const Position2& pos):
	Enemy(ply, bf,ef, pos, 10.0f),handle_(handle),frame_(0)
{
	vel_ = { 0.0f,+move_speed };;
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
