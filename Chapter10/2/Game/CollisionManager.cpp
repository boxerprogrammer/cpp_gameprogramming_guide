#include "CollisionManager.h"
#include "Collider.h"
#include "CircleCollider.h"
#include"Actor.h"
#include<algorithm>


bool CollisionManager::IsHit(const Circle& cA, const Circle& cB) const
{
	return (cB.pos-cA.pos).Magnitude()<=cA.radius+cB.radius;
}

void CollisionManager::CheckCollision()
{
	//‘“–‚½‚è”»’è
	for (int i = 0; i < colliders_.size()-1; ++i) {
		for (int j = i + 1; j < colliders_.size(); ++j) {
			auto colA = colliders_[i];
			auto colB = colliders_[j];
			if (colA->GetOwner()->GetType() == colB->GetOwner()->GetType()) {
				continue;
			}
			if (colA->GetType() == CollisionType::circle && 
				colB->GetType() == CollisionType::circle) {
				auto cCollA = dynamic_pointer_cast<CircleCollider>(colA);
				auto cCollB = dynamic_pointer_cast<CircleCollider>(colB);
				if (cCollA == nullptr || cCollB == nullptr) {
					continue;
				}
				const auto& circleA = cCollA->GetCircle();
				const auto& circleB = cCollB->GetCircle();
				if (IsHit(circleA, circleB)) {
					cCollA->OnHit(cCollB);
					cCollB->OnHit(cCollA);
					if (cCollA->GetRigidType() != RigidType::none &&
						cCollB->GetRigidType() != RigidType::none) {
						auto vec = circleB.pos - circleA.pos;
						auto len = vec.Magnitude();
						vec.Normalize();
						auto actorA = cCollA->GetOwner();
						auto actorB = cCollB->GetOwner();
						auto overlap = circleA.radius + circleB.radius - len;
						if (cCollA->GetRigidType() == RigidType::movable &&
							cCollB->GetRigidType() == RigidType::movable) {
							actorB->Move(vec * overlap * 0.5f);
							actorA->Move(-vec * overlap * 0.5f);
						}
						else {
							if (cCollA->GetRigidType() == RigidType::unmovable) {
								actorB->Move(vec * overlap);
							}
							else if(cCollB->GetRigidType() == RigidType::unmovable) {
								actorA->Move(-vec * overlap);
							}
						}
					}
				}
			}
		}
	}
}

void CollisionManager::Update()
{
	for (auto& coll : colliders_) {
		coll->Update();
	}
}

void CollisionManager::Sweep()
{
	auto it = std::remove_if(colliders_.begin(), colliders_.end(),
		[](const std::shared_ptr< Collider> coll) {
			return coll->IsDead();
		});
	colliders_.erase(it, colliders_.end());
}

void CollisionManager::AddCollider(std::shared_ptr<Collider> coll)
{
	colliders_.push_back(coll);
}
