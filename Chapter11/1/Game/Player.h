#pragma once
#include"Actor.h"
class Input;
class EffectFactory;
/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public Actor
{
private:
	std::shared_ptr<EffectFactory> effectFactory_;
	int handle_;//プレイヤーの画像
	int imgIndex_=0;//何番目の画像を表示するのか
	int resurrectFrame_=0;//復活中フレーム
	using Update_t = void(Player::*)();
	Update_t update_;
	void NormalUpdate();
	void ResurrectingUpdate();
public:
	Player(std::shared_ptr<EffectFactory> ef);
	bool IsResurrecting()const;
	void Update(Input& input);
	void AdjustPlayerPosition(Position2& pos, const Size& wsize);
	void Update()override;
	void Draw()override;
	void OnHit(std::shared_ptr<Actor>)override;
	void KillPlayer();
};

