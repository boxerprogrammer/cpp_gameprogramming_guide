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
#include"../Game/Shot.h"
#include"../Game/Background.h"
#include"../Game/Enemy.h"
#include"../Game/Bullet.h"
#include"../Game/EnemyFactory.h"
#include"../Game/EffectFactory.h"
#include"../Game/BulletFactory.h"
#include"../Game/ItemFactory.h"
#include"../Game/Stage.h"
#include"../Game/Collision.h"
constexpr int fade_interval = 20;

void GameScene::InitUpdate(Input&)
{
	auto loader = Application::GetInstance().GetLoader();

	effectFactory_ = std::make_shared<EffectFactory>();
	player_ = std::make_shared<Player>(effectFactory_);
	bulletFactory_ = std::make_shared<BulletFactory>(effectFactory_);

	for (auto& shot : shots_) {
		shot = std::make_shared<Shot>();
		shot->Init();
	}
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
	enemyFactory_->Update();
	effectFactory_->Update();
	bulletFactory_->Update();
	itemFactory_->Update();
	for (auto& shot : shots_) {
		shot->Update();
	}

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
	if (input.IsTriggered("shot")) {
		for (auto& shot : shots_) {
			if (shot->IsDead()) {
				shot->Fire(player_->GetPosition());
				break;
			}
		}
	}
	//敵と自機弾の当たり判定
	auto& enemies = enemyFactory_->GetEnemies();
	for (auto& enemy : enemies) {
		if (enemy->IsDead())continue;
		for (auto& shot : shots_) {
			if (shot->IsDead())continue;
			//それぞれの弾・敵を当たり判定する
			if (IsHit(shot->GetCollision(), enemy->GetCollision())) {
				//当たっていればActorのOnHitを呼び出す
				shot->OnHit(enemy);//お互いのアクターを知らせる
				enemy->OnHit(shot);//お互いのアクターを知らせる
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
	for (auto& shot : shots_) {
		if (shot->IsDead())continue;
		if (stage_->IsHitTerrain(shot)) {
			shot->OnHit(std::shared_ptr<Actor>());
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
			Position2 enemyPos = { wsize.w,static_cast<float>(yIdx * 32) };
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
	for (auto& shot : shots_) {
		shot->Draw();
	}
	player_->Draw();
	enemyFactory_->Draw();
	effectFactory_->Draw();
	bulletFactory_->Draw();
	itemFactory_->Draw();
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
	Application::GetInstance().GetLoader()->DeleteTemporalAssets();

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
