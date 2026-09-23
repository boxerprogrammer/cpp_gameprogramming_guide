#include "Enemy.h"
#include "Effect.h"
#include"EffectFactory.h"
#include"../Application.h"
constexpr int vanish_margin = 128;
void Enemy::CheckVanish()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
		//画面外に出たら弾を削除
	if ((circle_.pos.x < -vanish_margin) || (circle_.pos.x > wsize.w + vanish_margin) || 
		(circle_.pos.y < -vanish_margin) || (circle_.pos.y > wsize.h + vanish_margin)) {//弾は画面外に出たら死ぬ
		isDead_ = true;
	}
}

Enemy::Enemy(std::shared_ptr<Player> player,
	std::shared_ptr<BulletFactory> bf,
	std::shared_ptr<EffectFactory> ef,
		const Position2& pos, float r):
	bulletFactory_(bf),
	effectFactory_(ef),
	circle_{pos,r}
{
	actorType_ = ActorType::enemy;
	player_ = player;
	pos_ = pos;
}

void Enemy::SetPosition(const Position2& pos)
{
	pos_ = pos;
	circle_.pos = pos;
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

void Enemy::Damage(int damage)
{
	life_ -= damage;
	if (life_ <= 0) {
		isDead_ = true;
		OnDead();
	}
}
