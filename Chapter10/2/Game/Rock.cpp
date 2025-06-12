#include "Rock.h"
#include<DxLib.h>
#include"CircleCollider.h"

Rock::Rock(const Position2& pos):Actor(pos)
{
	coll_ = std::make_shared<CircleCollider>(this, 10.0f,  RigidType::unmovable);
}

void Rock::OnHit(const Actor* targetAct)
{
}

void Rock::Update()
{
}

void Rock::Draw()
{
	DrawCircleAA(pos_.x, pos_.y, 10, 32, 0xaa4444, true);
}

bool Rock::IsDead() const
{
	return false;
}

ActorType Rock::GetType() const
{
	return ActorType::rock;
}
