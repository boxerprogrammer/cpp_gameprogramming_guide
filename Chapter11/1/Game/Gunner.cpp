#include<DxLib.h>
#include "Gunner.h"
#include"../Scene/GameScene.h"
#include"Stage.h"
#include"Player.h"
#include"BulletFactory.h"

constexpr float enemy_speed = 3.0f;
constexpr int enemy_img_width = 36;
constexpr int enemy_img_height = 32;
constexpr float enemy_img_scale = 1.5f;
constexpr int enemy_walk_img_num = 8;//敵のアニメーション枚数(Walk)
constexpr int enemy_attack_img_num = 5;//敵のアニメーション枚数(Attack)
constexpr int enemy_img_interval = 3;//敵のアニメーション画像１枚当たり何フレーム表示するのか
constexpr float bullet_speed = 5.0f;//発射される弾の速度
constexpr int out_screen_margin = 100;
constexpr float enemy_hit_radius = 20.0f;
constexpr float attackable_distance = 200.0f;//攻撃を判断する距離
constexpr int shoot_interval = 20;
constexpr int shoot_num = 5;

constexpr int turn_x = 300;
constexpr int walk_idx_y = 1;
constexpr int attack_idx_y = 2;

constexpr int move_interval = 90;//移動時間
constexpr int idle_interval = 10;//待機時間
constexpr float gravity = 0.1f;
Gunner::Gunner(int handle, const Position2& pos, std::weak_ptr<Player> player, std::shared_ptr<EffectFactory> ef, std::shared_ptr<BulletFactory> bf, std::weak_ptr<GameScene> gs):
	Enemy(handle,pos,enemy_hit_radius,player,ef,bf),
	gameScene_(gs),
	basicPos_(pos)
{
	update_ = &Gunner::IdleUpdate;
	initScroll_=GetScroll();
	
}

void Gunner::CalculateVelocity()
{
	auto& pos = GetPosition();
	if (!player_.expired()) {
		vel_ = player_.lock()->GetPosition() - pos;
		vel_.y = 0.0;
		vel_.Normalize();
	}
}

int Gunner::GetScroll()
{
	if (!gameScene_.expired()) {
		auto pGameScene = gameScene_.lock();
		return pGameScene->GetStage()->GetCurrentScroll();
	}
	return 0;
}

void Gunner::DecideUpdate()
{
	CalculateVelocity();
	auto& pos = GetPosition();
	//もしプレイヤーが近くにいれば攻撃態勢に入る
	if (!player_.expired()) {
		const auto& ppos = player_.lock()->GetPosition();
		if (fabsf(ppos.x - pos.x) < attackable_distance) {
			update_ = &Gunner::AttackUpdate;
			thinkFrame_ = 0;
			return;
		}
	}
	//そうでなければ距離を詰める
	update_ = &Gunner::WalkUpdate;
}

void Gunner::WalkUpdate()
{
	auto& pos = GetPosition();
	pos.x = basicPos_.x - (GetScroll() - initScroll_);
	basicPos_ += vel_;
	if (++thinkFrame_ >= move_interval) {
		update_ = &Gunner::IdleUpdate;
		thinkFrame_ = 0;
	}
}

void Gunner::IdleUpdate()
{
	auto& pos = GetPosition();
	pos.x = basicPos_.x - (GetScroll() - initScroll_);
	if (++thinkFrame_ >= idle_interval) {
		update_ = &Gunner::DecideUpdate;
	}
}
void Gunner::AttackUpdate()
{

	auto& pos = GetPosition();
	pos.x = basicPos_.x - (GetScroll() - initScroll_);

	if (player_.expired()) {
		thinkFrame_ = 0;
		update_ = &Gunner::IdleUpdate;
		return;
	}
	
	if (thinkFrame_ % shoot_interval == 0) {
		const auto& ppos = player_.lock()->GetPosition();//プレイヤーの座標
		const float h = std::max(pos.y-ppos.y,0.0f);//最終的にsqrtするので0を下回らないように
		float vx = vel_.x;
		float vy = -std::max(fabsf(vel_.x), 0.1f);
		Vector2 vel = { vx,vy };
		vel = vel.Normalized() * bullet_speed;
		vel.y = -sqrtf(2.0* gravity*h);

		bulletFactory_->Create(pos, vel, BulletType::cannon_ball);
	}
	if (thinkFrame_ > shoot_interval * shoot_num) {
		thinkFrame_ = 0;
		update_ = &Gunner::IdleUpdate;
		return;
	}
	++thinkFrame_;
}


void Gunner::Update()
{
	++frame_;
	(this->*update_)();
}

void Gunner::Draw()
{
	const auto& pos = GetPosition();

	int xIdx = (frame_ / enemy_img_interval) % enemy_walk_img_num;
	int yIdx = walk_idx_y;
	if (update_ == &Gunner::AttackUpdate) {
		xIdx = (frame_ / enemy_img_interval) % enemy_attack_img_num;
		yIdx = attack_idx_y;
	}
	DrawRectRotaGraph(pos.x, pos.y,
		xIdx * enemy_img_width, yIdx * enemy_img_height,
		enemy_img_width, enemy_img_height,
		enemy_img_scale, 0.0f,
		handle_, true,
		vel_.x>0.0f);
					
}
