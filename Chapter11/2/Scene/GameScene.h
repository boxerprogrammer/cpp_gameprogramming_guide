#pragma once
#include"../Geometry.h"
#include "Scene.h"
#include<memory>
#include<array>
class Player;
class Shot;
class Background;
class EnemyFactory;
class EffectFactory;
class BulletFactory;
class ItemFactory;
class ShotFactory;
class Stage;
class Force;
/// <summary>
/// ゲーム中のシーン
/// </summary>
class GameScene : public Scene,public std::enable_shared_from_this<GameScene>
{
private:
	std::shared_ptr<Player> player_;
	std::shared_ptr<ShotFactory> shotFactory_;
	std::shared_ptr<Background> background_;
	std::shared_ptr<EnemyFactory> enemyFactory_;
	std::shared_ptr<EffectFactory> effectFactory_;
	std::shared_ptr<BulletFactory> bulletFactory_;
	std::shared_ptr<ItemFactory> itemFactory_;
	std::shared_ptr<Stage> stage_;
	std::shared_ptr<Force> force_;
	
	int indicatorH_;

	int handle_;//仮のハンドル
	std::array<int,3> backImgs_;//背景画像
	int frame_;//


	using UpdateFunc_t = void(GameScene::*)(Input&);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;
	void InitUpdate(Input&);
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	void NormalDraw();
	void FadeDraw();
	void EmptyDraw();

public:
	GameScene(SceneController& ctrl);
	~GameScene();
	void Update(Input& input)override;
	void Draw()override;
	const std::shared_ptr<Stage> GetStage()const;
	const std::shared_ptr<EnemyFactory> GetEnemyFactory()const;
};

