#include "GameScene.h"
#include<DxLib.h>
#include<cassert>
#include<cmath>
#include"../Application.h"
#include"../FileLoader.h"
#include"../Input.h"

#include"GameoverScene.h"
#include"TitleScene.h"
#include"SceneController.h"
#include"PauseScene.h"

#include"../Game/Player.h"
#include"../Game/Force.h"
#include"../Game/Shot.h"
#include"../Game/Background.h"
#include"../Game/Enemy.h"
#include"../Game/Bullet.h"
#include"../Game/EnemyFactory.h"
#include"../Game/EffectFactory.h"
#include"../Game/BulletFactory.h"
#include"../Game/ItemFactory.h"
#include"../Game/ShotFactory.h"
#include"../Game/Stage.h"
#include"../Game/Collision.h"
constexpr int fade_interval = 20;
constexpr int gui_margin = 50;

void GameScene::InitUpdate(Input&)
{
	auto loader = Application::GetInstance().GetLoader();

	effectFactory_ = std::make_shared<EffectFactory>();
	shotFactory_ = std::make_shared<ShotFactory>(shared_from_this());
	player_ = std::make_shared<Player>(effectFactory_,shotFactory_);

	force_ = std::make_shared<Force>(loader->LoadGraph(L"img/Actor/force.png"),
									player_,
									shared_from_this());
	player_->SetForce(force_);
	
	bulletFactory_ = std::make_shared<BulletFactory>(effectFactory_);
	itemFactory_ = std::make_shared<ItemFactory>(shared_from_this());
	enemyFactory_ = std::make_shared<EnemyFactory>(player_,
													effectFactory_,
													bulletFactory_,
													itemFactory_,
													shared_from_this());
	
	background_ = std::make_shared<Background>();

	


	stage_ = std::make_shared<Stage>();
	stage_->Load(1);//ステージ１をロード

	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
}

void GameScene::FadeInUpdate(Input&)
{
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		frame_ = 0;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	background_->Update();
	stage_->Update();
	player_->Update(input);
	force_->Update();
	enemyFactory_->Update();
	effectFactory_->Update();
	bulletFactory_->Update();
	itemFactory_->Update();
	shotFactory_->Update();

	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frame_ = 0;
		return;
	}
	if (input.IsTriggered("back")) {
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}
	//if (input.IsTriggered("shot")) {
	//	shotFactory_->Fire(player_->GetPosition(),ShotType::normal);
	//}
	//敵と自機弾の当たり判定
	auto& shots = shotFactory_->GetShots();
	auto& enemies = enemyFactory_->GetEnemies();
	for (auto& enemy : enemies) {
		if (enemy->IsDead())continue;

		for (auto& shot : shots) {
			if (shot->IsDead())continue;
			//それぞれの弾・敵を当たり判定する
			if (IsHit(shot->GetCollision(), enemy->GetCollision())) {
				//当たっていればActorのOnHitを呼び出す
				shot->OnHit(enemy);//お互いのアクターを知らせる
				enemy->OnHit(shot);//お互いのアクターを知らせる
			}
		}
	}

	//フォースと敵、フォースと弾の当たり判定
	if (!force_->IsDead()) {
		for (auto& enemy : enemies) {
			if (enemy->IsDead())continue;
			//当たり判定して
			if (IsHit(force_->GetCollision(), enemy->GetCollision())) {
				enemy->OnHit(force_);
			}
		}
		auto& bullets = bulletFactory_->GetBullets();
		for (auto& bullet : bullets) {
			if (bullet->IsDead())continue;
			if (IsHit(force_->GetCollision(), bullet->GetCollision())) {
				bullet->OnHit(force_);
			}
		}
	}

	if (!player_->IsDead()&&!player_->IsResurrecting()) {
		//敵と自機の当たり判定
		for (auto& enemy : enemies) {
			if (enemy->IsDead())continue;
			//当たり判定して
			if (IsHit(player_->GetCollision(), enemy->GetCollision())) {
				player_->OnHit(enemy);//当たったら反応する
				enemy->OnHit(player_);
			}
		}
		auto& bullets = bulletFactory_->GetBullets();
		//敵の弾と自機の当たり判定
		for (auto& bullet : bullets) {
			if (bullet->IsDead()) {
				continue;
			}
			if (IsHit(player_->GetCollision(), bullet->GetCollision())) {
				player_->OnHit(bullet);
				bullet->OnHit(player_);
			}
		}
	}

	


	for (auto& item : itemFactory_->GetItems()) {
		if (IsHit(player_->GetCollision(), item->GetCollision())) {
			player_->OnHit(item);
			item->OnHit(player_);
			break;
		}
	}

	//地形との当たり判定
	//地形と自機の当たり判定
	if (stage_->IsHitTerrain(player_)) {
		player_->OnHit(std::shared_ptr<Actor>());
	}
	for (auto& shot : shots) {
		if (shot->IsDead())continue;
		std::shared_ptr<Collision> terrainCol;
		if (stage_->IsHitTerrain(shot, terrainCol)) {
			shot->OnHit(terrainCol);
		}
	}
	

	//現在のスクロールに合わせて敵を出現させる
	auto scroll = stage_->GetCurrentScroll();
	//チップごとにマップの読み直しを行う
	const auto chipWidth = stage_->GetChipWidth();
	if (scroll % chipWidth  == 0) {//チップサイズ分進むまでは特に何もしない
		auto colNo = (scroll / chipWidth) + chipWidth;//ひとつ右の列
		//データを１列分ロードする
		auto column = stage_->GetColumn(LayerType::enemy,colNo);
		for (int yIdx = 0;yIdx < column.size();++yIdx) {
			auto enemyType = static_cast<EnemyType>(column[yIdx]);
			if (enemyType == EnemyType::none) {
				continue;
			}
			//敵の出現場所は右端から出現
			//(もし左から出現するような敵の場合は敵じたいにそういう機能を
			// 追加しておく)
			Position2 enemyPos = { static_cast<float>(wsize.w),static_cast<float>(yIdx * 32) };
			enemyFactory_->CreateEnemy(enemyPos,enemyType);
		}
	}
		
	
}

void GameScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
		return;
	}
}

void GameScene::NormalDraw()
{
	background_->Draw();
	stage_->Draw();
	DrawString(10, 10, L"Game Scene...", 0xffffff);
	shotFactory_->Draw();
	player_->Draw();
	force_->Draw();
	enemyFactory_->Draw();
	effectFactory_->Draw();
	bulletFactory_->Draw();
	itemFactory_->Draw();

	const auto& wsize = Application::GetInstance().GetWindowSize();
	//簡易版のゲージを表示
	constexpr int charge_gauge_width = 300;
	constexpr int charge_gauge_height = 20;

	const float chargeRate = player_->GetChargingRate();
	//チャージゲージ本体
	DrawBox(gui_margin, wsize.h - gui_margin - charge_gauge_height,
		gui_margin + charge_gauge_width*chargeRate, wsize.h - gui_margin,
		0xaaaaff,true);

	//チャージゲージ枠
	DrawBox(gui_margin, wsize.h - gui_margin - charge_gauge_height,
		gui_margin + charge_gauge_width, wsize.h - gui_margin,
		0xffffff,false,2);

}

void GameScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	NormalDraw();//通常の描画

	//アルファの割合を計算する(0.0<=rate<=1.0)
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	//黒セロファンを濃度(アルファ値)を変えながら描画する
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::EmptyDraw()
{
}

GameScene::GameScene(SceneController& ctrl):Scene(ctrl)
{
	update_ = &GameScene::InitUpdate;
	draw_ = &GameScene::EmptyDraw;
	frame_ = fade_interval;
}

GameScene::~GameScene()
{
	Application::GetInstance().GetLoader()->DeleteTemporaryAssets();

}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}

const std::shared_ptr<Stage> GameScene::GetStage() const
{
	return stage_;
}

const std::shared_ptr<EnemyFactory> GameScene::GetEnemyFactory() const
{
	return enemyFactory_;
}

const std::shared_ptr<ShotFactory> GameScene::GetShotFactory() const
{
	return shotFactory_;
}
