#include "Bullet.h"
#include"Collision.h"

Bullet::Bullet(int handle ,const Position2& pos, float radius):
	Actor(pos,radius),
	handle_(handle)
{
	type_ = ActorType::bullet;
}

void Bullet::OnHit(const std::shared_ptr<Actor> actor)
{
	auto& pos = coll_->GetCircle().pos;
	if (actor->GetType() == ActorType::player || IsOutScreen(pos)) {
		isDead_ = true;
	}
}
