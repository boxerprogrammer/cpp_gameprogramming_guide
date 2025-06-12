#include "Ball.h"
#include"CircleCollider.h"
#include<DxLib.h>

Ball::Ball(const Position2& pos) :Actor(pos)
{
	coll_ = std::make_shared<CircleCollider>(this,10.0f,RigidType::movable);
}

void Ball::OnHit(const Actor* targetAct)
{
}

void Ball::Update()
{
}

void Ball::Draw()
{
	DrawCircleAA(pos_.x, pos_.y, 10, 32, 0xaaffaa, true);
}

bool Ball::IsDead() const
{
	return false;
}

ActorType Ball::GetType()const
{
	return ActorType::ball;
}
