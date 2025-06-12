#include "Enemy.h"
#include"CircleCollider.h"
#include<DxLib.h>

Enemy::Enemy(const Position2& pos) :Actor(pos)
{
	coll_ = std::make_shared< CircleCollider>(this, 10.0f);
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (frame_ > 0) {
		--frame_;
	}
}

void Enemy::Draw()
{
	DrawCircleAA(pos_.x, pos_.y, 10, 32, 0xffaaaa);
	if (frame_ > 0) {
		DrawFormatString(500, 10, 0xffffff, L"time : %d", frame_);
	}
}

bool Enemy::IsDead()const
{
	return false;
}

void Enemy::OnHit(const Actor* targetAct)
{
	frame_ = 60;
}

ActorType Enemy::GetType() const
{
	return ActorType::enemy;
}
