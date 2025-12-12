#include "GameScene.h"
#include<DxLib.h>
#include"../Input.h"
#include"GameoverScene.h"
#include"SceneController.h"
#include"PauseScene.h"
#include"../Application.h"
//ゲーム中に使用するオブジェクト
#include"../Game/Stage.h"
#include"../Game/Player.h"
#include"../Game/EnemyFactory.h"
#include"../Game/BulletFactory.h"
#include"../Game/EffectFactory.h"
#include"../Game/Enemy.h"
#include"../Game/Bullet.h"
#include<cassert>
#include<cmath>

constexpr int player_cut_w = 16;
constexpr int player_cut_h = 24;
constexpr int bullet_cut_w = 16;
constexpr int bullet_cut_h = 16;
constexpr int exp_cut_w = 16;
constexpr int exp_cut_h = 16;

constexpr int fade_interval = 60;
constexpr float player_speed = 4.0f;//プレイヤーの移動速度
constexpr float player_shot_speed = 8.0f;//プレイヤーの弾速度
constexpr float player_scale = 2.0f;//プレイヤー画像のスケーリング
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
playerCircle_{ {320,400},10.0f },
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
	//使用する画像をロード
	playerH_ = LoadGraph(L"resource/img/game/player.png");
	assert(playerH_ >= 0);
	shotH_ = LoadGraph(L"resource/img/game/bullet.png");
	assert(shotH_ >= 0);
	backH_ = LoadGraph(L"resource/img/game/background.png");
	assert(backH_ >= 0);
	cloudH_ = LoadGraph(L"resource/img/game/cloud.png");
	assert(cloudH_ >= 0);
	frame_ = fade_interval;

	//ステージデータのロード
	stage_ = std::make_shared<Stage>();
	stage_->Load(1);//1ステージ目を読み込む

	player_ = std::make_shared<Player>();
	bulletFactory_ = std::make_shared<BulletFactory>();
	effectFactory_ = std::make_shared<EffectFactory>();
	enemyFactory_ = std::make_shared< EnemyFactory>(bulletFactory_,effectFactory_);

	
}

void 
GameScene::FadeInUpdate(Input&) {
	if (--frame_ <= 0) {
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
		frame_ = 0;
	}

	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}

	Vector2 vel = {};//プレイヤーの速度ベクトル
	if (input.IsPressed("up")) {
		vel.y = -1.0f;
	}
	if (input.IsPressed("right")) {
		vel.x = 1.0f;
	}
	if (input.IsPressed("down")) {
		vel.y = 1.0f;
	}
	if (input.IsPressed("left")) {
		vel.x = -1.0f;
	}
	//爆発中は死んでるから動けない
	if (playerExplosionFrame_ == 0) {
		playerCircle_.pos += vel.Normalized() * player_speed;
	}

	//プレイヤーの弾の制御
	if (input.IsTriggered("shot")) {
		for (auto& shot : shots_) {
			//弾発射の儀
			if (shot.isDead) {
				shot.isDead = false;//弾を復活
				shot.circle.pos = playerCircle_.pos;//弾の開始地点
				shot.vel = { 0.0f,-player_shot_speed };//弾の速度
				break;
			}
		}
	}
	
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//敵の生成
	if (scroll_ == topIdx_ * 32) {
		auto dataW = wsize.w / 32;
		const auto& stageData=stage_->GetAllData();
		const auto& mapSize = stage_->MapSize();
		const auto& wsize = Application::GetInstance().GetWindowSize();
		int topOffset = wsize.h / cell_size;
		auto idxY = (topIdx_ + topOffset);
		if (idxY >= mapSize.h) {
			isStageEnd_ = true;
		}
		else {
			for (int idxX = 0; idxX < dataW; ++idxX) {
				auto enemyType = stageData[idxX + idxY * mapSize.w];
				if ((EnemyType)enemyType == EnemyType::none) {
					continue;
				}
				enemyFactory_->Create(idxX, 15,
					static_cast<EnemyType>(enemyType),
					player_);
			}
			++topIdx_;
		}
	}
	if (!isStageEnd_) {
		++scroll_;
		//敵の弾の制御(自機狙い弾)
		++gameFrame_;
	}
	enemyFactory_->Update();
	effectFactory_->Update();
	////敵の動きの制御
	//enemy_.pos += enemyVel_;
	//if ((enemyVel_.x > 0.0f && enemy_.pos.x > wsize.w - enemy_cut_w*enemy_scale/2.0f) ||
	//	(enemyVel_.x < 0.0f && enemy_.pos.x < enemy_cut_w * enemy_scale/2.0f)) {
	//	enemyVel_.x = -enemyVel_.x;
	//}
	//
	//if (gameFrame_ < 120) {
	//	if (gameFrame_ % 20 == 0) {//20フレームに１発弾を撃つ
	//		for (auto& bullet : bullets_) {
	//			//敵弾発射の儀
	//			if (bullet.isDead) {
	//				bullet.isDead = false;
	//				bullet.circle.pos = enemy_.pos;
	//				bullet.vel = (playerCircle_.pos - enemy_.pos).Normalized() * enemy_bullet_speed;//自機狙い弾
	//				break;
	//			}
	//		}
	//	}
	//}
	//else {//n-wayショット
	//	if (gameFrame_ % 20 == 0) {//20フレームに１発弾を撃つ
	//		//真ん中は自機狙い
	//		//両隣は30°左右に分かれてるとします
	//		constexpr int way_num = 9;//何wayか
	//		constexpr float angle_30 = DX_PI_F / 15.0f;
	//		//いったん自機狙いの角度を計算します
	//		auto vec = playerCircle_.pos - enemy_.pos;//自機狙いベクトル
	//		auto theta = atan2(vec.y, vec.x)- angle_30*(way_num/2);
	//		int count = 0;
	//		for (auto& bullet : bullets_) {
	//			//敵弾発射の儀
	//			if (bullet.isDead) {
	//				bullet.isDead = false;
	//				bullet.circle.pos = enemy_.pos;
	//				//bullet.vel = (playerPos_ - enemyPos_).Normalized() * enemy_bullet_speed;//自機狙い弾
	//				bullet.vel = { cos(theta),sin(theta) };
	//				bullet.vel *= enemy_bullet_speed;
	//				theta += angle_30;
	//				++count;
	//			}
	//			if (count == way_num) {
	//				break;
	//			}
	//		}
	//	}
	//}

	//プレイヤー弾の座標の更新
	for (auto& shot : shots_) {
		if (shot.isDead)continue;
		shot.circle.pos += shot.vel;
		if (shot.circle.pos.y < -bullet_cut_h * player_scale) {
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
					effectFactory_->Create(efkPos, EffectType::explosion);
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



	enemyExplosionFrame_ = max(enemyExplosionFrame_ - 1, 0);

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
		if (playerExplosionFrame_ == 0) {
			if (IsHit(bullet.circle, playerCircle_)) {
				bullet.isDead = true;
				playerExplosionFrame_ = explosion_frame;
			}
		}
	}
	playerExplosionFrame_= max(playerExplosionFrame_ - 1, 0);
	
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
	DrawRectRotaGraph(playerCircle_.pos.x, playerCircle_.pos.y,
		player_cut_w * 2, player_cut_h * 0,
		player_cut_w, player_cut_h,
		player_scale, 0.0f, playerH_, true);
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
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
			DrawRectRotaGraph(shot.circle.pos.x, shot.circle.pos.y,//表示座標
				bullet_cut_w * shotImgIdx, bullet_cut_h * 1,//切り取りの左上
				bullet_cut_w, bullet_cut_h,//切り取りの幅高
				player_scale, 0.0f, shotH_, true);
			shot.circle.Draw();
		}
	}
	
	//自機表示
	if (playerExplosionFrame_ == 0) {//爆発してない時だけ表示
		int playerImgIdx = (gameFrame_ / 8) % 2;
		DrawRectRotaGraph(playerCircle_.pos.x, playerCircle_.pos.y,//プレイヤーの表示座標
			player_cut_w * 2, player_cut_h * playerImgIdx,//切り取りの左上
			player_cut_w, player_cut_h,//切り取りの幅高
			player_scale, 0.0f, playerH_, true);
	}

	//爆発表示
	if (playerExplosionFrame_ > 0) {
		int idx = 4 - (playerExplosionFrame_ / 6);
		DrawRectRotaGraph(playerCircle_.pos.x, playerCircle_.pos.y,//プレイヤーの表示座標
			exp_cut_w * idx, 0,
			exp_cut_w, exp_cut_h,
			exp_scale, 0.0f, explosionH_, true);
	}

	//敵弾表示
	for (auto& bullet : bullets_) {
		if (bullet.isDead) continue;
		DrawRectRotaGraph(bullet.circle.pos.x, bullet.circle.pos.y,
			bullet_cut_w * shotImgIdx, bullet_cut_h * 0,
			bullet_cut_w, bullet_cut_h,
			enemy_scale, 0.0f, shotH_, true);
	}


	//敵爆発表示
	int enemyExpOffsetX = GetRand(10) - 5;
	int enemyExpOffsetY = GetRand(10) - 5;
	if (enemyExplosionFrame_ > 0) {
		int idx = 4 - (enemyExplosionFrame_ / 6);
		DrawRectRotaGraph(
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

}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
