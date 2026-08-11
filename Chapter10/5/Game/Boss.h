#pragma once
#include "Enemy.h"
#include"../Geometry.h"
class Boss :
    public Enemy
{
private:
	int handle_;//画像ハンドル(Factoryからもらったもの)
	float angle_ = 0.0f;
	float radius_ = 50.0f;
	int frame_ = 0;//経過フレーム
	Vector2 vel_ = {};//敵の速度ベクトル
	using Update_t = void (Boss::*)();
	using Draw_t = void (Boss::*)();
	Update_t update_;
	Draw_t draw_;
	void AppearUpdate();
	void NormalUpdate();
	void DyingUpdate();
	void NormalDraw();
	void DyingDraw();
public:
	Boss(int handle, std::shared_ptr<Player> player, std::shared_ptr<BulletFactory> bulletFactory, std::shared_ptr<EffectFactory> effectFactory, Vector2 pos);
	void Update() override;
	void Draw() override;
};

