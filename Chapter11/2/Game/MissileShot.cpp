#include "MissileShot.h"
#include<DxLib.h>
#include"../Scene/GameScene.h"
#include"EnemyFactory.h"
#include"Enemy.h"

constexpr float shot_speed = 8.0f;
constexpr float homing_rate = 0.1f;//どれくらいホーミングするかの割合
constexpr float direct_attack_radius = 20.0f;//相手にどれくらい近づいたら直接あたりに行くか

constexpr float homing_speed = shot_speed * homing_rate;
constexpr int shot_img_width = 32;
constexpr int shot_img_height = 32;
constexpr float missile_radius = 10.0f;
constexpr float shot_img_scale = 2.0f;


constexpr int anim_interval = 5;//アニメーションの切り替えフレーム
constexpr int anim_num = 6;

MissileShot::MissileShot(int handle, const Position2& pos, std::weak_ptr<GameScene> gs):
	Shot(handle,pos, missile_radius),
	gameScene_(gs)
{
	vel_ = { shot_speed ,0.0f };
	SelectTarget();
}

void MissileShot::SelectTarget()
{
	if (!gameScene_.expired()) {
		//敵のリストを持ってきて
		//死んでない者をターゲットとする
		const auto& enemies = gameScene_.lock()->GetEnemyFactory()->GetEnemies();
		for (auto enemy : enemies) {
			//すでにロックオンされてる敵は狙わない
			//(ただしロックオンフラグが立たない敵もいる)
			if (!enemy->IsDead() && !enemy->IsLockedOn()) {
				target_ = enemy;
				return;
			}
		}
	}
}

void MissileShot::Update()
{
	auto& pos = GetPosition();
	pos += vel_;
	
	if (!target_.expired()) {
		//今のミサイルの場所から敵へ向かうベクトル
		auto homingVec = target_.lock()->GetPosition() - pos;
		//ある程度敵に近づいたら直接あたりに行く
		if (homingVec.Length() < direct_attack_radius) {//相手にそのまま向かう
			vel_ = homingVec.Normalized() * shot_speed;
		}
		else {//ゆっくり相手に向かう
			homingVec = homingVec.Normalized() * homing_speed;//ホーミング調整
			vel_ = (vel_ + homingVec).Normalized() * shot_speed;//
		}
	}

	++frame_;
}

void MissileShot::Draw()
{
	auto& pos = GetPosition();
	int idx = (frame_ / anim_interval) % anim_num;

	//atan2を用いて、現在のミサイルの進行方向を元に
	//角度を計算して言いる。
	auto angle = atan2(vel_.y, vel_.x);

	DrawRectRotaGraph(pos.x, pos.y,
		idx * shot_img_width, 0,
		shot_img_width, shot_img_height,
		shot_img_scale, angle,
		handle_, true);
}

void MissileShot::OnHit(std::shared_ptr<Actor>)
{
	isDead_ = true;
}
