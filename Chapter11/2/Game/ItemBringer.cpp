#include<DxLib.h>
#include "ItemBringer.h"
#include"EffectFactory.h"
#include"ItemFactory.h"
#include"../Scene/GameScene.h"
#include"Stage.h"
#include"../Application.h"
constexpr float enemy_speed = 2.0f;
constexpr int enemy_img_width = 48;
constexpr int enemy_img_height = 48;
constexpr float enemy_img_scale = 1.5f;
constexpr int enemy_walk_img_num = 4;//敵のアニメーション枚数
constexpr int enemy_img_interval = 3;//敵のアニメーション画像１枚当たり何フレーム表示するのか
constexpr int out_screen_margin = 100;
constexpr float enemy_hit_radius = 16.0f;


ItemBringer::ItemBringer(int handle,
						const Position2& pos, 
						std::weak_ptr<Player> player, 
						std::shared_ptr<EffectFactory> ef, 
						std::shared_ptr<BulletFactory> bf,
						std::weak_ptr<GameScene> gs,
						std::weak_ptr<ItemFactory> itemFact):
	Enemy(handle,pos, enemy_hit_radius,player,ef,bf),
	gameScene_(gs),
	itemFactory_(itemFact)
{
	vel_ = { -enemy_speed,0.0f };
	update_ = &ItemBringer::ForwardUpdate;
}


void ItemBringer::ForwardUpdate()
{
	auto& pos = GetPosition();
	pos += vel_;
	if (gameScene_.expired()) {
		return;
	}
	auto stage = gameScene_.lock()->GetStage();
	//壁に当たりました→壁をよけるモードに移行
	if (stage->IsHitTerrain(shared_from_this())) {
		avoidFrame_ = 0;
		update_ = &ItemBringer::AvoidUpdate;
		const auto& wsize = Application::GetInstance().GetWindowSize();
		const auto centerY = wsize.h / 2;//画面中央Y
		auto& pos = GetPosition();
		pos.x += 1.0f;
		//壁をよけて中央に動くようにする
		//ただし２回目も同じ方向に動く(最初が下だったらずっと下)
		//そうしないと画面中央で行ったり来たりしてしまう
		if (avoidVel_.y == 0.0f) {//初回だけどちらに動くかを決めておく
			avoidVel_ = { 0.0f,centerY - pos.y };
			avoidVel_.Normalize();
			if (avoidVel_.y == 0.0f) {
				avoidVel_.y = 1.0f;
			}
			avoidVel_ *= enemy_speed;
		}
	}
}
//壁をよけるように動く
void ItemBringer::AvoidUpdate()
{
	auto& pos = GetPosition();
	pos += avoidVel_;
	if (gameScene_.expired()) {
		return;
	}
	if (++avoidFrame_ >= 10) {
		auto stage = gameScene_.lock()->GetStage();
		if (!stage->IsHitTerrain(shared_from_this())) {
			update_ = &ItemBringer::ForwardUpdate;
		}
		avoidFrame_ = 0;
	}
}

void ItemBringer::Update()
{
	(this->*update_)();
	++frame_;
	CheckOutScreen();
}

void ItemBringer::Draw()
{
	const auto& pos = GetPosition();
	int idx = (frame_ / enemy_img_interval)%enemy_walk_img_num;
	DrawRectRotaGraph(pos.x, pos.y,
		idx * enemy_img_width, 0,
		enemy_img_width, enemy_img_height,
		enemy_img_scale, 0.0f,
		handle_, true);
}

void ItemBringer::OnHit(std::shared_ptr<Actor> actor)
{
	

	isDead_ = true;
	if (IsDead()) {
		//死んだらそこでザコ爆発エフェクトを表示する
		effectFactory_->Emit(GetPosition(), EffectType::zako_explosion);
		//ToDo : ここでアイテムも落とすようにする
		if (!itemFactory_.expired()) {
			const auto& pos = GetPosition();
			static bool isFirst = true;//最初の１回しか実行されない
			//アイテムを生む
			int itemno = isFirst ? 
							static_cast<int>(ItemType::missile) 
								: 
							GetRand(3) + 1;//1～4までの数値をランダムに得る
			isFirst = false;//毎回実行される
			itemFactory_.lock()->Create(pos, static_cast<ItemType>(itemno));
		}
	}
}
