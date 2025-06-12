#include "Item.h"
#include"CircleCollider.h"

#include<DxLib.h>

Item::Item(const Position2& pos):Actor(pos)
{
	coll_ = std::make_shared< CircleCollider>(this, 10.0f);
}

Item::~Item()
{
	coll_->Kill();
}

void Item::Update()
{
	if (frame_ > 0) {
		--frame_;
	}
}

void Item::Draw()
{
	if (isGot_)return;
	DrawCircleAA(pos_.x, pos_.y, 10, 32, 0x00ffff, true);
	if (frame_ > 0) {
		DrawFormatString(10, 400, 0xffffff, L"time : %d", frame_);
	}
}

ActorType Item::GetType() const
{
	return ActorType::item;
}

bool Item::IsDead()const
{
	return isGot_;
}

void Item::OnHit(const Actor* targetAct)
{
	frame_ = 60;
	isGot_ = true;
}
