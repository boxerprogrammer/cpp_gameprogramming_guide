#include "Actor.h"

Actor::Actor(const Position2& pos, float radius)
{
	coll_ = std::make_shared<Collision>(pos, radius);
}

ActorType Actor::GetActorType()const { 
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
	if (coll_->GetType() == CollisionType::circle) {
		return coll_->GetCircle().pos;
	}
	else {
		return coll_->GetCapsule().pos;
	}
}

std::shared_ptr<Collision> Actor::GetCollision()
{
	return coll_;
}

bool Actor::IsDead() const
{
	return isDead_;
}
