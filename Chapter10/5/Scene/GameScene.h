#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<memory>

//プロトタイプ宣言
class GameUI;//ゲームUI
class PlayerStatus;//プレイヤーステータス
class Score;//スコア
class Stage;//ステージ
class Player;//プレイヤー
class EnemyFactory;//敵ファクトリ
class BulletFactory;//弾ファクトリ
class EffectFactory;//エフェクトファクトリ

class GameScene : public Scene , public std::enable_shared_from_this<GameScene>
{
private:
	//レイヤーの種別
	enum class LayerType {
		enemy=0,//敵配置データ
		event=1//イベントデータ
	};
	enum class EventType {
		none = 0,
		stop10sec,//10秒ストップ

	};

	bool isStageEnd_ = false;//ステージの最後に到達しました
	//弾構造体(後で別のクラスに分ける)
	struct Shot {
		Circle circle = { {},3.0f };//弾の座標
		Vector2 vel;//弾の速度
		bool isDead=true;//無効になってる
	};
	Shot shots_[32];//プレイヤーの弾

	//イベント系
	int scrollStopTimer_ = 0;

	//敵情報
	Circle enemy_= { {0.0f,0.0f},10.0f };//敵の座標
	int enemyExplosionFrame_=0;
	Vector2 enemyVel_;//敵の速度
	Shot bullets_[128];//敵の弾
	int shotSE_=0;
	int explosionSE_ = 0;

	int effResHandle_ = -1;
	int effPlayHandle_ = -1;

	int topIdx_=0;//現在のステージデータのトップ
	int scroll_=-1;//スクロール値

	std::shared_ptr<GameUI> gameUI_;//ゲームUI
	std::shared_ptr<PlayerStatus> playerStatus_;//プレイヤーステータス
	std::shared_ptr<Score> score_;//スコア
	std::shared_ptr<Stage> stage_;//ステージデータ

	std::shared_ptr<Player> player_;//プレイヤー
	std::shared_ptr<EnemyFactory> enemyFactory_;//敵生産工場
	std::shared_ptr<BulletFactory> bulletFactory_;//弾生産工場(仮)
	std::shared_ptr<EffectFactory> effectFactory_;//エフェクト生産工場

	//使用する画像ハンドル
	int playerH_;//プレイヤー画像(4x2)
	int shotH_;//プレイヤーのショット(敵の弾も兼用)
	int explosionH_;//爆発画像

	int backH_;//背景画像
	int cloudH_;//多重スクロール用雲画像

	int frame_=0;//フェードインアウト用
	int gameFrame_=0;//ゲーム中のフレームをカウント

	int hitStopFrame_ = 0;

	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void HitStopUpdate(Input& input);
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(GameScene::*)(Input&);
	UpdateFunc_t update_;//Update系を受け取るメンバ関数ポインタ

	void FadeDraw();
	void NormalDraw();
	void HitStopDraw();
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t draw_;//Draw系を受け取るメンバ関数ポインタ
	bool IsHit(const Circle& a, const Circle& b);

	int renderTarget_;//Draw関数が書き込める書き込み先
	int renderTargetBokeh_;//ボケ画像保管用

public:
	GameScene(SceneController& controller);
	~GameScene();
	void DoHitStop();
	void Update(Input&)override;
	void Draw()override;
};

