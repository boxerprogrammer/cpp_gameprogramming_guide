#include "Zako.h"
#include"Player.h"
#include"BulletFactory.h"
#include"NormalBullet.h"
#include"../Application.h"
#include"Collision.h"
#include<DxLib.h>
#include<cmath>
constexpr float enemy_speed = 3.0f;
constexpr int enemy_img_width = 29;
constexpr int enemy_img_height = 29;
constexpr float enemy_img_scale = 2.0f;
constexpr int enemy_walk_img_num = 5;//敵のアニメーション枚数
constexpr int enemy_img_interval = 5;//敵のアニメーション画像１枚当たり何フレーム表示するのか
constexpr float bullet_speed = 5.0f;//発射される弾の速度
constexpr int out_screen_margin = 100;
constexpr float enemy_hit_radius = 16.0f;
constexpr int turn_x = 300;

constexpr float wave_degree_per_frame = 6.0f;//６度

void Zako::ForwardUpdate()
{
	auto& pos = coll_->GetCircle().pos;

	vel_.x = -1.0;//前進
	vel_.y = sin(angle_);//yのほうはsinカーブを描く
	vel_ = vel_.Normalized() * enemy_speed;//波打って動く
	pos += vel_;//現在の座標に加算する

	angle_ += DegreeToRadian(wave_degree_per_frame);//１フレームに6度ずつ動く

	++frame_;
	//もし画面右端に到達したら状態を後退モードに移行する
	if (pos.x <= turn_x) {
		update_ = &Zako::BackUpdate;//後退モードに移行
		vel_ = { enemy_speed,0.0f };//速度を右向きにする
		frame_ = 0;
		//ここで、自機狙い弾を撃つ
		if (player_.expired()) {
			return;
		}
		const auto& playerPos = player_.lock()->GetPosition();
		//終点(プレイヤー座標)から始点(自分の座標)を引く
		auto vel = (playerPos - pos).Normalized() * bullet_speed;
		bulletFactory_->Create(pos, vel, BulletType::normal);
		return;
	}
}

void Zako::BackUpdate()
{
	auto& pos = coll_->GetCircle().pos;
	pos += vel_;
	++frame_;
	if (IsOutScreen(pos)) {
		return;
	}
}

Zako::Zako(int handle, const Position2& pos,
			std::weak_ptr<Player> player,
			std::shared_ptr<EffectFactory> ef, 
			std::shared_ptr<BulletFactory> bf):
	Enemy(handle,pos, enemy_hit_radius,player,ef,bf)
{
	initPos_ = pos;
	update_ = &Zako::ForwardUpdate;//最初は前進する
}

void Zako::Update()
{
	(this->*update_)();
}

void Zako::Draw()
{
	auto& pos = GetPosition();
	int xidx = (frame_ / enemy_img_interval) % enemy_walk_img_num;
	DrawRectRotaGraph(
		pos.x, pos.y,
		enemy_img_width * xidx, 0,
		enemy_img_width, enemy_img_height,
		enemy_img_scale, 0.0f,
		handle_, true);
}
