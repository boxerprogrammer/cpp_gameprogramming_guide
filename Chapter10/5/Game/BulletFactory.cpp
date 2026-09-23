#include "BulletFactory.h"
#include"Bullet.h"
#include"../ResourceManager.h"
#include<algorithm>
#include<DxLib.h>
BulletFactory::BulletFactory()
{
	handle_ = mylib::LoadTexture(L"img/game/enemy_bullet.png");
}

void BulletFactory::Create(std::shared_ptr<Actor> owner, const Position2& pos,const Vector2& vel)
{
	bullets_.push_back(std::make_shared<Bullet>(owner, handle_,pos, vel));
}

void BulletFactory::Clear()
{
	bullets_.clear();
}

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
