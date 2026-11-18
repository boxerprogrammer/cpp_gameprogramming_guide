#include "Enemy.h"
#include"../Application.h"
#include"../FileLoader.h"
#include"EffectFactory.h"
#include"BulletFactory.h"
#include"Player.h"
#include<DxLib.h>
#include<cmath>


Enemy::Enemy(int handle,const Position2& pos,float radius,
			std::weak_ptr<Player> player,
			std::shared_ptr<EffectFactory> ef,
			std::shared_ptr<BulletFactory> bf) :
	Actor(pos,radius),
	handle_(handle),
	player_(player),
	effectFactory_(ef),
	bulletFactory_(bf)
{
	type_ = ActorType::enemy;
}

void Enemy::OnHit(std::shared_ptr<Actor>)
{
	isDead_ = true;
	if (IsDead()) {
		//死んだらそこでザコ爆発エフェクトを表示する
		effectFactory_->Emit(GetPosition(), EffectType::zako_explosion);
	}
}

void Enemy::Init()
{
	CollisionSet();
}
