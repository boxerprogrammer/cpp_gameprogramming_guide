#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<memory>

//プロトタイプ宣言
class Stage;//ステージ
class Player;//プレイヤー
class EnemyFactory;//敵ファクトリ
class BulletFactory;//弾ファクトリ
class EffectFactory;//エフェクトファクトリ

class GameScene : public Scene
{
private:
	bool isStageEnd_ = false;//ステージの最後に到達しました
	//プレイヤー情報
	Circle playerCircle_ = { {0.0f,0.0f},10.0f };//プレイヤーの座標
	int playerExplosionFrame_=0;
	//弾構造体(後で別のクラスに分ける)
	struct Shot {
		Circle circle = { {},3.0f };//弾の座標
		Vector2 vel;//弾の速度
		bool isDead=true;//無効になってる
	};
	Shot shots_[32];//プレイヤーの弾

	//敵情報
	Circle enemy_= { {0.0f,0.0f},10.0f };//敵の座標
	int enemyExplosionFrame_=0;
	Vector2 enemyVel_;//敵の速度
	Shot bullets_[128];//敵の弾

	int topIdx_=0;//現在のステージデータのトップ
	int scroll_=-1;//スクロール値

	std::shared_ptr<Stage> stage_;//ステージデータ
	std::shared_ptr<Player> player_;//プレイヤー(仮)
	std::shared_ptr<EnemyFactory> enemyFactory_;//敵生産工場
	std::shared_ptr<BulletFactory> bulletFactory_;//弾生産工場(仮)
	std::shared_ptr<EffectFactory> effectFactory_;//エフェクト生産工場

	//使用する画像ハンドル
	int playerH_;//プレイヤー画像(4x2)
	int shotH_;//プレイヤーのショット(敵の弾も兼用)
	int zakoH_;//敵の画像
	int patapataH_;//敵2の画像
	int explosionH_;//爆発画像

	int backH_;//1枚絵
	int cloudH_;//多重スクロール用雲画像

	int frame_=0;//フェードインアウト用
	int gameFrame_=0;//ゲーム中のフレームをカウント

	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(GameScene::*)(Input&);
	UpdateFunc_t update_;//Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t draw_;//Draw系を受け取るメンバ関数ポインタ
	bool IsHit(const Circle& a, const Circle& b);
public:
	GameScene(SceneController& controller);
	void Update(Input&)override;
	void Draw()override;
};

