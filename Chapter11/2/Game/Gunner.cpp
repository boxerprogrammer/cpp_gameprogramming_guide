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
constexpr float hop_speed_y = 2.0f;//小ジャンプYスピード
constexpr float jump_speed_y = 5.0f;//大ジャンプYスピード
constexpr float hop_speed_x = 1.0f;//小ジャンプXスピード
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
		//X方向のみプレイヤーに向かって動くようにする
		vel_ = player_.lock()->GetPosition() - pos;
		vel_.y = 0.0;
		if (vel_.x == 0.0f) {
			vel_.x = -1.0f;//もし動かない状態ならスクロールする方向に動くようにする
		}
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
	//プレイヤーに向かって動くように速度を計算する
	CalculateVelocity();
	auto& pos = GetPosition();
	//もしプレイヤーが近くにいれば攻撃態勢に入る
	if (!player_.expired()) {
		//プレイヤーとのX差分を計算します
		const auto& ppos = player_.lock()->GetPosition();
		//一定範囲内(X軸のみ見てる)に入ったら弾発射モードに入る
		if (fabsf(ppos.x - pos.x) < attackable_distance) {
			update_ = &Gunner::AttackUpdate;
			thinkFrame_ = 0;
			return;
		}
	}
	//そうでなければ距離を詰める(今向いてる方向に歩く)
	update_ = &Gunner::WalkUpdate;
}

void Gunner::HopForDescentUpdate()
{
	
	auto& pos = GetPosition();
	pos.x = AdjustScrollX();//スクロールを考慮したX座標にしておく
	basicPos_ += vel_;
	pos += vel_;
	vel_.y += gravity;
	if (!gameScene_.expired()) {
		auto stage = gameScene_.lock()->GetStage();
		auto lastPos = pos;
		if (stage->BlockTerrain(shared_from_this())) {
			auto overlap = (pos - lastPos);
			basicPos_ += overlap;//basicPos_も補正する
			CalculateVelocity();
			update_ = &Gunner::WalkUpdate;
			return;
		}
	}

}

void Gunner::JumpUpdate()
{
	auto& pos = GetPosition();
	pos.x = AdjustScrollX();//スクロールを考慮したX座標にしておく
	basicPos_ += vel_;
	pos += vel_;
	vel_.y += gravity;
	if (!gameScene_.expired()) {
		auto stage = gameScene_.lock()->GetStage();
		auto lastPos = pos;
		if (stage->BlockTerrain(shared_from_this())) {
			auto overlap = (pos - lastPos);
			basicPos_ += overlap;//basicPos_も補正する
			CalculateVelocity();
			update_ = &Gunner::WalkUpdate;
			return;
		}
	}
}

void Gunner::WalkUpdate()
{
	//DecideUpdateのCalculateVelocityですでにどちらに向かうかを
	//計算しているので、ただその方向に向かって動く
	auto& pos = GetPosition();
	pos.x = AdjustScrollX();//スクロールを考慮したX座標にしておく
	basicPos_ += vel_;//基準となる座標に移動値を加算
	if (++thinkFrame_ >= move_interval) {
		update_ = &Gunner::IdleUpdate;
		thinkFrame_ = 0;
	}
	
	if(!gameScene_.expired()){
		auto stage = gameScene_.lock()->GetStage();
		float sign = vel_.x / fabsf(vel_.x);
		//目の前の床があるかどうかを判定しなければ小ジャンプを行う
		Vector2 offset = { sign * enemy_img_width * enemy_img_scale,enemy_img_height * enemy_img_scale };
		bool isHit = stage->IsHitTerrain(shared_from_this(), offset);
		//当たってれば「地面がある」そうでないなら、地面がないので小ジャンプします
		if (!isHit) {
			vel_.x = hop_speed_x * sign;
			vel_.y = -hop_speed_y;
			update_=&Gunner::HopForDescentUpdate;//下降のためにちょっとジャンプ
			return;
		}

		//目の前に壁があるかどうかを判定し、あれば大ジャンプを行って飛び越え試行をする
		offset = { sign * enemy_img_width * enemy_img_scale,0.0f};
		isHit = stage->IsHitTerrain(shared_from_this(), offset);
		if (isHit) {
			vel_.x = hop_speed_x * sign;
			vel_.y = -jump_speed_y;
			update_ = &Gunner::JumpUpdate;//がっつりジャンプ
			return;
		}
	}
	
}

//スクロールを考慮した現在地を返す
float Gunner::AdjustScrollX()
{
	return basicPos_.x - (GetScroll() - initScroll_);
}

void Gunner::IdleUpdate()
{
	auto& pos = GetPosition();
	pos.x = AdjustScrollX();
	if (++thinkFrame_ >= idle_interval) {
		update_ = &Gunner::DecideUpdate;
	}
}
//放物線弾を5発発射して、またアイドル状態に戻る
void Gunner::AttackUpdate()
{
	auto& pos = GetPosition();
	pos.x = AdjustScrollX();

	if (player_.expired()) {
		thinkFrame_ = 0;
		update_ = &Gunner::IdleUpdate;
		return;
	}
		
	if (thinkFrame_ % shoot_interval == 0) {
		const auto& ppos = player_.lock()->GetPosition();//プレイヤーの座標
		const float h = std::max(pos.y-ppos.y,0.0f);//最終的にsqrtするので0を下回らないように
		float vx = vel_.x;//進行方向のXベクトル
		Vector2 vel = { vx,0.0f };
		//どこまで打ち上げるのか(プレイヤーに当たるところまで打ち上げる)
		vel = vel.Normalized() * bullet_speed;
		//V₀=√2gh
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
	CheckOutScreen();
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
