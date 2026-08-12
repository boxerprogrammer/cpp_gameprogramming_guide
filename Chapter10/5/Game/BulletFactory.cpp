#include "BulletFactory.h"

BulletFactory::BulletFactory()
{}

void BulletFactory::Create(std::shared_ptr<Actor> actor, const Position2& pos,const Vector2& dir)
{}

void BulletFactory::Update()
{
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	bullets_.remove_if([](std::shared_ptr<Bullet> bullet) {
		return bullet->IsDead();
		});
}

void BulletFactory::Draw()
{
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}
