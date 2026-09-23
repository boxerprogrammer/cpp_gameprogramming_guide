#include "Strider.h"
#include"Player.h"
#include<DxLib.h>
#include"BulletFactory.h"
constexpr int enemy_cut_w = 32;
constexpr int enemy_cut_h = 32;
constexpr float enemy_scale = 2.5f;
constexpr int one_picture_frame = 10;//1つの絵を表示するフレーム
constexpr int picture_num = 2;//アニメーションに必要な絵の枚数
constexpr float bullet_speed = 6.0f;//弾のスピード
constexpr float way_angle = DX_PI_F / 6.0f;//弾の角度
constexpr int bullet_interval = 60;
Strider::Strider(int handle, std::shared_ptr<Player> ply,
				std::shared_ptr<BulletFactory> bf, 
				std::shared_ptr<EffectFactory> ef,
	const Position2& pos):
frame_(0),
handle_(handle),
Enemy(ply,bf,ef,pos,20.0f)
{
	life_ = 3;

}

void Strider::Update()
{
	++frame_;
	pos_.y += 3.0f;
	circle_.pos = pos_;
	if(frame_%bullet_interval==0){
		//3-wayショット
		auto vel = player_->GetPos()-pos_;
		float angle = atan2f(vel.y, vel.x);
		angle -= way_angle;
		for(int i=0;i<3;++i){
			vel = { cosf(angle),sinf(angle) };
			vel *= bullet_speed;
			bulletFactory_->Create(shared_from_this(), pos_, vel);
			angle += way_angle;
		}
	}
	CheckVanish();
}

void Strider::Draw()
{
	int idx = (frame_ / one_picture_frame) % picture_num;
	//一定時間ごとに絵を切り替えて表示
	DrawRectRotaGraph(pos_.x, pos_.y,
		enemy_cut_w * idx, 0,//32ピクセルずらしてアニメーション
		enemy_cut_w, enemy_cut_h,
		enemy_scale, 0.0f,
		handle_, true);
}

void Strider::OnHit(const Actor& actor)
{
	if (--life_ <= 0) {
		isDead_ = true;
		OnDead();
	}
}
