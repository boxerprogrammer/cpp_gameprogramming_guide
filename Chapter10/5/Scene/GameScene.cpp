#include "GameScene.h"
#include<DxLib.h>
#include"../Input.h"
#include"../ResourceManager.h"
#include"GameoverScene.h"
#include"SceneController.h"
#include"PauseScene.h"
#include"../Application.h"
//ゲーム中に使用するオブジェクト
#include"../Game/Score.h"
#include"../Game/Stage.h"	
#include"../Game/Player.h"
#include"../Game/EnemyFactory.h"
#include"../Game/BulletFactory.h"
#include"../Game/EffectFactory.h"
#include"../Game/Enemy.h"
#include"../Game/Bullet.h"

#include"../Game/PlayerStatus.h"
#include"../Game/GameUI.h"
#include<cassert>
#include<cmath>


constexpr float player_shot_scale = 2.0f;//プレイヤー弾画像のスケーリング


constexpr int bullet_cut_w = 16;
constexpr int bullet_cut_h = 16;
constexpr int exp_cut_w = 16;
constexpr int exp_cut_h = 16;

constexpr int fade_interval = 60;

constexpr int explosion_frame = 30;//爆発に要するフレーム数
constexpr float exp_scale = 4.0f;//爆発スケール

constexpr float enemy_speed = 2.0f;
constexpr float enemy_bullet_speed = 4.0f;
constexpr int enemy_cut_w = 16;
constexpr int enemy_cut_h = 16;
constexpr float enemy_scale = 2.5f;

constexpr int cell_size = 32;

/// <summary>
/// ２つの円が交差しているかどうかを返す
/// </summary>
/// <param name="a">円a</param>
/// <param name="b">円b</param>
/// <returns>当たってたらtrue / 当たってないならfalse</returns>
bool 
GameScene::IsHit(const Circle& a, const Circle& b)
{
	//①円aの中心から円bの中心へベクトルを引く
	auto vec = b.pos - a.pos;
	//②①で作ったベクトルの大きさを測る
	float d = vec.Length();
	//③その大きさがそれぞれの円の半径を足したものより大きければ
	//当たっていない
	return d<=a.r+b.r;
}

GameScene::GameScene(SceneController& controller):Scene(controller),
enemy_{ {320,50},10.0f },
enemyVel_{enemy_speed,0.0f},
update_(&GameScene::FadeInUpdate),
draw_(&GameScene::FadeDraw),
shots_{},
bullets_{}
{
	for (auto& shot : shots_) {
		shot.circle.r = 8.0f;
	}

	shotH_ = mylib::LoadTexture(L"img/game/bullet.png");
	assert(shotH_ >= 0);
	backH_ = mylib::LoadTexture(L"img/game/background.png");
	assert(backH_ >= 0);
	cloudH_ = mylib::LoadTexture(L"img/game/cloud.png");
	assert(cloudH_ >= 0);

	shotSE_ = LoadSoundMem(L"se/shot.wav");
	explosionSE_ = LoadSoundMem(L"se/explosion.wav");
	frame_ = fade_interval;

	//ステージデータのロード
	stage_ = std::make_shared<Stage>();
	stage_->Load(1);//1ステージ目を読み込む


	//GameScene自身のthisのshared_ptrを渡す
	score_ = std::make_shared<Score>();
	player_ = std::make_shared<Player>(*this);
	playerStatus_ = std::make_shared<PlayerStatus>();
	bulletFactory_ = std::make_shared<BulletFactory>();
	effectFactory_ = std::make_shared<EffectFactory>();
	enemyFactory_ = std::make_shared< EnemyFactory>(player_,bulletFactory_,effectFactory_);
	gameUI_.reset(new GameUI(*score_.get(), *playerStatus_.get(), *controller.GetGameData().get()));

	const auto& wsize = Application::GetInstance().GetWindowSize();
	renderTarget_ = MakeScreen(wsize.w, wsize.h);
	renderTargetBokeh_ = MakeScreen(wsize.w, wsize.h);
}

GameScene::~GameScene()
{
	DeleteGraph(renderTarget_);
	DeleteGraph(renderTargetBokeh_);
	DeleteSoundMem(shotSE_);
}

void GameScene::DoHitStop()
{
	hitStopFrame_ = 30;
	update_ = &GameScene::HitStopUpdate;
	draw_ = &GameScene::HitStopDraw;
}

void 
GameScene::FadeInUpdate(Input&) {
	if (--frame_ <= 0) {
		PlayMusic(L"bgm/bgm.wav",DX_PLAYTYPE_LOOP);
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		return;
	}
}
void 
GameScene::NormalUpdate(Input& input) {
	
	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		StopMusic();
		frame_ = 0;
	}

	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}


	//爆発中は死んでるから動けない
	player_->Update(input);

	//プレイヤーの弾の制御
	if (input.IsTriggered("shot")) {
		for (auto& shot : shots_) {
			//弾発射の儀
			if (shot.isDead) {
				shot.isDead = false;//弾を復活
				shot.circle.pos = player_->GetPos();//弾の開始地点
				shot.vel = { 0.0f,-8.0f };//弾の速度
				PlaySoundMem(shotSE_, DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//敵の生成
	if (scroll_ == topIdx_ * 32) {
		auto dataW = wsize.w / 32;
		const auto& enemyData=stage_->GetAllData()[static_cast<int>(LayerType::enemy)];
		const auto& eventData = stage_->GetAllData()[static_cast<int>(LayerType::event)];
		const auto& mapSize = stage_->MapSize();
		const auto& wsize = Application::GetInstance().GetWindowSize();
		int topOffset = wsize.h / cell_size;
		auto idxY = (topIdx_ + topOffset);
		if (idxY >= mapSize.h) {
			isStageEnd_ = true;
		}
		else {
			for (int idxX = 0; idxX < dataW; ++idxX) {
				auto enemyType = enemyData[idxX + idxY * mapSize.w];
				if ((EnemyType)enemyType != EnemyType::none) {
					enemyFactory_->Create(idxX, 15,
						static_cast<EnemyType>(enemyType));
				}
				auto eventType = eventData[idxX + idxY * mapSize.w];
				switch (static_cast<EventType>(eventType)) {
				case EventType::stop10sec:
					scrollStopTimer_ = 600;
					break;
				default:
					continue;
				}
			}
			++topIdx_;
		}
		
	}
	//下限0でデクリメントする
	scrollStopTimer_ = std::max(scrollStopTimer_ - 1, 0);
	if (!isStageEnd_ && scrollStopTimer_==0) {
		++scroll_;
		//敵の弾の制御(自機狙い弾)
		++gameFrame_;
	}
	
	enemyFactory_->Update();
	effectFactory_->Update();
	
	//プレイヤー弾の座標の更新
	for (auto& shot : shots_) {
		if (shot.isDead)continue;
		shot.circle.pos += shot.vel;
		if (shot.circle.pos.y < -bullet_cut_h * player_shot_scale) {
			shot.isDead = true;//画面外に出たらまた無効にする(再利用可能にする)
			continue;
		}
		//プレイヤー弾と敵の当たり判定
		auto& enemies = enemyFactory_->GetEnemies();
		for (auto& enm : enemies) {
			if (IsHit(shot.circle, enm->GetCollision())) {
				shot.isDead = true;
				Bullet blt;
				enm->OnHit(blt);
				auto efkPos = (shot.circle.pos + enm->GetCollision().pos) * 0.5f;
				if (enm->IsDead()) {
					PlaySoundMem(explosionSE_, DX_PLAYTYPE_BACK);
					effectFactory_->Create(efkPos, EffectType::explosion);
					score_->Add(enm->GetScore());//スコア加算
				}
				else {
					effectFactory_->Create(efkPos, EffectType::damage);
				}
				break;
			}
		}
	}
	auto& enemies = enemyFactory_->GetEnemies();
	auto it = std::remove_if(enemies.begin(), enemies.end(),
								[](auto enm) {
									return enm->IsDead();
								});
	enemies.erase(it, enemies.end());



	enemyExplosionFrame_ = std::max(enemyExplosionFrame_ - 1, 0);

	//敵の弾の座標更新
	for (auto& bullet : bullets_) {
		if (bullet.isDead)continue;
		bullet.circle.pos += bullet.vel;
		if (bullet.circle.pos.x < -32 || bullet.circle.pos.x > 640 + 32 || 
			bullet.circle.pos.y < -32 || bullet.circle.pos.y >480 + 32) {//弾は画面外に出たら死ぬ
			bullet.isDead = true;
			continue;
		}
		//敵の弾と自機の当たり判定
		if (!player_->IsDead()) {
			if (IsHit(bullet.circle, player_->GetCollision())) {
				bullet.isDead = true;
				//player_->OnHit()
			}
		}
	}
	if (!player_->IsDead()) {
		//プレイヤーと敵の当たり判定
		for (auto& enemy : enemies) {
			if (IsHit(enemy->GetCollision(), player_->GetCollision())) {
				player_->OnHit(*enemy);
				enemy->OnHit(*player_);
				break;
			}
		}
	}
}
void GameScene::HitStopUpdate(Input& input)
{
	if (--hitStopFrame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}
void 
GameScene::FadeOutUpdate(Input&) {
	if (++frame_ >=fade_interval) {
		//以下の行を実行した時点でGameSceneは消滅します
		controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
		//もう死んでるのでreturn以外のコードを実行してはいけない
		return;//もうChangeSceneした直後にreturnする癖をつけておいてください
	}
}

void 
GameScene::FadeDraw() {
	//背景
	DrawExtendGraph(0, 0, 640, 480, backH_, false);
	//自機表示
	player_->Draw();
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void 
GameScene::NormalDraw() {
	const auto& wsize = Application::GetInstance().GetWindowSize();
	constexpr int cloud_height = 160;
	int cloudYOffset = (gameFrame_ * 5) % wsize.h;//雲
	int bgYOffset = (gameFrame_*2)% wsize.h;//画面の高さぶん進んだらオフセット座標をリセット
	//背景(永遠スクロールの場合必要な枚数は、一度に表示する枚数＋１)
	//↓０フレームの時は、画面上に映っていません
	DrawExtendGraph(0, -wsize.h + bgYOffset, wsize.w,  bgYOffset, backH_, false);
	//常に移っているけれども画面の高さぶん進んだ時点で見えなくなる
	DrawExtendGraph(0, 0 + bgYOffset, wsize.w, wsize.h + bgYOffset, backH_, false);

	//雲
	//↓０フレームの時は、画面上に映っていません
	DrawExtendGraph(0, cloudYOffset- wsize.h, wsize.w, cloud_height+cloudYOffset- wsize.h, cloudH_, true);
	//常に移っているけれども画面の高さぶん進んだ時点で見えなくなる
	DrawExtendGraph(0, 0 + cloudYOffset, wsize.w, cloud_height + cloudYOffset, cloudH_, true);

	//弾表示
	int shotImgIdx = (gameFrame_ / 5) % 2;
	for (auto& shot : shots_) {
		if (!shot.isDead) {
			DrawRectRotaGraphF(shot.circle.pos.x, shot.circle.pos.y,//表示座標
				bullet_cut_w * shotImgIdx, bullet_cut_h * 1,//切り取りの左上
				bullet_cut_w, bullet_cut_h,//切り取りの幅高
				player_shot_scale , 0.0f, shotH_, true);
			shot.circle.Draw();
		}
	}
	


	//自機爆発表示


	//敵弾表示
	for (auto& bullet : bullets_) {
		if (bullet.isDead) continue;
		DrawRectRotaGraphF(bullet.circle.pos.x, bullet.circle.pos.y,
			bullet_cut_w * shotImgIdx, bullet_cut_h * 0,
			bullet_cut_w, bullet_cut_h,
			enemy_scale, 0.0f, shotH_, true);
	}


	//敵爆発表示
	int enemyExpOffsetX = GetRand(10) - 5;
	int enemyExpOffsetY = GetRand(10) - 5;
	if (enemyExplosionFrame_ > 0) {
		int idx = 4 - (enemyExplosionFrame_ / 6);
		DrawRectRotaGraphF(
			enemy_.pos.x+enemyExpOffsetX, 
			enemy_.pos.y+enemyExpOffsetY,//プレイヤーの表示座標
			exp_cut_w * idx, 0,
			exp_cut_w, exp_cut_h,
			1.5f, 0.0f, explosionH_, true);
	}


	//ロードした敵の表示
	enemyFactory_->Draw();

	//エフェクト表示
	effectFactory_->Draw();

	//自機表示
	player_->Draw();

	//UI表示
	//gameUI_->Draw();

}

void GameScene::HitStopDraw()
{
	SetDrawScreen(renderTarget_);//これで自作のキャンバスに描画が行われる
	NormalDraw();//いつもどおりの描画
	SetDrawScreen(DX_SCREEN_BACK);//元々ある、裏画面に戻す

	GraphFilterBlt(renderTarget_, renderTargetBokeh_, DX_GRAPH_FILTER_GAUSS, 16, 1400);

	int x = GetRand(10) - 5;
	int y = GetRand(10) - 5;
	int x2 = GetRand(15) - 5;
	int y2 = GetRand(20) - 10;
	DrawGraph(x, y, renderTarget_, false);
}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
