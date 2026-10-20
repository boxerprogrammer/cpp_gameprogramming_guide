#pragma once
#include "Enemy.h"
#include"../Geometry.h"
#include "Actor.h"
class GameScene;
class Boss :
    public Enemy
{
private:
	int handle_;//画像ハンドル(Factoryからもらったもの)
	float angle_ = 0.0f;
	float radius_ = 50.0f;
	int frame_ = 0;//経過フレーム
	Vector2 vel_ = {};//敵の速度ベクトル
	GameScene& gameScene_;
	using Update_t = void (Boss::*)();
	using Draw_t = void (Boss::*)();
	Update_t update_;
	Update_t bulletsUpdate_;
	Draw_t draw_;
	void AppearUpdate();
	void SimpleBulletsUpdate();
	void MovePosition();
	void NWayBulletsUpdate();
	void RadialBulletsUpdate();
	void ScatteredBulletsUpdate();
	void RainyBulletsUpdate();
	void NormalUpdate();
	void DyingUpdate();
	void NormalDraw();
	void DyingDraw();

	void ShootNWayBullets(int wayNum, float angle);
	void ShootRadialBullets(int wayNum, float angle);

public:
	Boss(GameScene& gameScene, int handle, std::shared_ptr<Player> player, std::shared_ptr<BulletFactory> bulletFactory, std::shared_ptr<EffectFactory> effectFactory, Vector2 pos);
	void Update() override;
	void Draw() override;
	void OnHit(const Actor& actor)override;
	void Damage(int damage)override;
};

