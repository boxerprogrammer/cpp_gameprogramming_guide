#include "Actor.h"

Actor::Actor(const Position2& pos):pos_(pos)
{
}

const Position2& Actor::GetPos() const
{
	return pos_;
}

void Actor::Move(const Vector2& vec)
{
	pos_ += vec;
}

std::shared_ptr<Collider> Actor::GetCollision()
{
	return coll_;
}
