#pragma once
#include"Geometry.h"
#include "Collider.h"
class CircleCollider :
    public Collider
{
private:
    Circle circle_;
public:
    CircleCollider(Actor* owner, float r,RigidType rgdType=RigidType::none);
    CollisionType GetType()const override;
    const Circle& GetCircle()const;
    void Update()override;
    void OnHit(std::shared_ptr<Collider> target)override;
};

