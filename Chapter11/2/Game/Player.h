#pragma once
#include"Actor.h"
class Input;
class EffectFactory;
class ShotFactory;
class Force;
/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public Actor
{
private:
	std::shared_ptr<EffectFactory> effectFactory_;
	std::shared_ptr<ShotFactory> shotFactory_;
	std::weak_ptr<Force> force_;
	int handle_;//プレイヤーの画像
	int imgIndex_=0;//何番目の画像を表示するのか
	int resurrectFrame_=0;//復活中フレーム
	using Update_t = void(Player::*)();
	float playerSpeed = 0.0f;
	Update_t update_;

	int chargingFrame_ = 0;

	bool hasMissile_ = false;//ミサイルの能力を持っている
	int missileFrame_=0;//ミサイルの撃てなくなってる時間

	bool hasReflectLaser_ = false;


	void NormalUpdate();
	void ResurrectingUpdate();
public:
	Player(std::shared_ptr<EffectFactory> ef,std::shared_ptr<ShotFactory> sf);
	bool IsResurrecting()const;
	void Update(Input& input);
	void AdjustPlayerPosition(Position2& pos, const Size& wsize);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor>)override;
	void KillPlayer();
	void SetForce(std::weak_ptr<Force> force);
	float GetChargingRate()const;
};

