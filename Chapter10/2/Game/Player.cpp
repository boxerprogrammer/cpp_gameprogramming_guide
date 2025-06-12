#include "Player.h"
#include"CircleCollider.h"
#include<DxLib.h>

Player::Player(const Position2& pos) :Actor(pos)
{
	coll_ = std::make_shared< CircleCollider>(this, 10.0f,RigidType::movable);
}

Player::~Player()
{
}

void Player::Update()
{
	char keystate[256];
	GetHitKeyStateAll(keystate);
	if (frame_ == 0) {
		if (keystate[KEY_INPUT_UP]) {
			pos_.y -= 4.0f;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			pos_.y += 4.0f;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			pos_.x -= 4.0f;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			pos_.x += 4.0f;
		}
	}
	if (frame_ > 0) {
		--frame_;
		if (frame_ == 0) {
			pos_ = { 320,240 };
		}
	}
}

void Player::Draw()
{
	
	if (frame_ > 0) {
		DrawCircleAA(pos_.x, pos_.y, 10, 32, 0xffffff, false);
		DrawFormatString(10, 10, 0xffffff, L"time : %d", frame_);
	}
	else {
		DrawCircleAA(pos_.x, pos_.y, 10, 32, 0xffffff, true);
	}
}

bool Player::IsDead()const
{
	return false;
}

void Player::OnHit(const Actor* targetAct)
{
	if (frame_ > 0)return;
	if (targetAct->GetType() == ActorType::enemy) {
		frame_ = 60;
	}
}

ActorType Player::GetType() const
{
	return ActorType::player;
}
