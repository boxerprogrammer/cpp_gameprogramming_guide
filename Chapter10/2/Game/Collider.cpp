#include "Collider.h"

Collider::Collider(Actor* owner):owner_(owner)
{
}

RigidType Collider::GetRigidType() const
{
    return rigidType_;
}


Actor* Collider::GetOwner() 
{
    return owner_;
}
