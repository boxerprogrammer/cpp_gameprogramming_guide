#include "CircleCollider.h"
#include"Actor.h"


CircleCollider::CircleCollider(Actor* owner, float r, RigidType rgdType):Collider(owner)
{
	circle_.radius = r;
	rigidType_ = rgdType;
}

CollisionType CircleCollider::GetType() const
{
	return CollisionType::circle;
}

const Circle& CircleCollider::GetCircle() const
{
	return circle_;
}

void CircleCollider::Update()
{
	circle_.pos = owner_->GetPos();
}

void CircleCollider::OnHit(const std::shared_ptr<Collider> target)
{
	owner_->OnHit(target->GetOwner());
}
