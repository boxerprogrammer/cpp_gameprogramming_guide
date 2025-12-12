#include "Enemy.h"
#include "Effect.h"
#include"EffectFactory.h"

Enemy::Enemy(std::shared_ptr<Player> player,
	std::shared_ptr<BulletFactory> bf,
	std::shared_ptr<EffectFactory> ef,
		const Position2& pos, float r):
	bulletFactory_(bf),
	effectFactory_(ef),
	circle_{pos,r}
{
	player_ = player;
	pos_ = pos;
}

const Circle& Enemy::GetCollision() const
{
	return circle_;
}


bool Enemy::IsDead() const
{
	return isDead_;
}

void Enemy::OnDead()
{
	effectFactory_->Create(pos_, EffectType::enemy_dead_1);
}
