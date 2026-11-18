#include "Actor.h"

Actor::Actor(const Position2& pos, float radius)
{
	coll_ = std::make_shared<Collision>(pos, radius);
}

ActorType Actor::GetType()const { 
	return type_; 
}

void Actor::SetPosition(const Position2& pos)
{
	coll_->GetCircle().pos = pos;
}

void Actor::CollisionSet()
{
	coll_->SetOwner(shared_from_this());
}

Position2& Actor::GetPosition()
{
	return coll_->GetCircle().pos;
}

std::shared_ptr<Collision> Actor::GetCollision()
{
	return coll_;
}

bool Actor::IsDead() const
{
	return isDead_;
}
