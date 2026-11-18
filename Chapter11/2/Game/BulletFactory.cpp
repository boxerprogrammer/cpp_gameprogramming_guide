#include "BulletFactory.h"
#include"Bullet.h"
#include"NormalBullet.h"
#include"CannonBall.h"
#include"../Application.h"
#include"../FileLoader.h"
#include<algorithm>
BulletFactory::BulletFactory(std::weak_ptr<EffectFactory> ef):
	effectFactory_(ef)
{
	auto loader = Application::GetInstance().GetLoader();
	bulletImgs_.push_back(loader->LoadGraph(L"img/Actor/bullet.png", AssetType::temporary));
	bulletImgs_.push_back(loader->LoadGraph(L"img/Actor/gunner.png", AssetType::temporary));


	createFuncs_.push_back([this](int handle, const Position2& pos, const Vector2& vel) {
		return std::make_shared<NormalBullet>(handle, pos, vel);
		});

	createFuncs_.push_back([this](int handle, const Position2& pos, const Vector2& vel) {
		return std::make_shared<CannonBall>(handle, pos, vel);
		});

}

void BulletFactory::Update()
{
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	std::erase_if(bullets_, [](const auto bullet) {
		return bullet->IsDead();
		});
}

void BulletFactory::Draw()
{
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

void BulletFactory::Create(const Position2& pos, const Vector2& vel, BulletType type)
{
	int idx = static_cast<int>(type);
	auto handle = bulletImgs_[idx];
	auto& createFunc = createFuncs_[idx];
	bullets_.push_back(createFunc(handle,pos,vel));
}

std::list<std::shared_ptr<Bullet>>& BulletFactory::GetBullets()
{
	return bullets_;
}
