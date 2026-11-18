#include "ShotFactory.h"
#include"../Application.h"
#include"../FileLoader.h"
#include"NormalShot.h"
#include"ChargeShot.h"
#include"MissileShot.h"
#include"ReflectLaserShot.h"

ShotFactory::ShotFactory(std::weak_ptr<GameScene> gs):gameScene_(gs)
{
	auto loader = Application::GetInstance().GetLoader();
	shotImgs_.push_back(loader->LoadGraph(L"img/Actor/shot.png", AssetType::temporary));
	shotImgs_.push_back(loader->LoadGraph(L"img/Actor/charge_shot_3.png", AssetType::temporary));
	shotImgs_.push_back(loader->LoadGraph(L"img/Actor/missile_shot.png", AssetType::temporary));
	shotImgs_.push_back(-1);//レーザーの画像は今はない

	//通常弾
	createFuncs_.push_back([](int handle, const Position2& pos) {
		return std::make_shared<NormalShot>(handle, pos);
		});
	//チャージショット
	createFuncs_.push_back([](int handle, const Position2& pos) {
		return std::make_shared<ChargeShot>(handle,pos);
	});
	//ミサイル弾
	createFuncs_.push_back([this](int handle, const Position2& pos) {
		return std::make_shared<MissileShot>(handle, pos,gameScene_);
		});
	//3-way反射レーザー
	createFuncs_.push_back([](int handle, const Position2& pos) {
		return std::shared_ptr<Shot>();
		});

}

void ShotFactory::Update()
{
	for (auto& shot : shots_) {
		shot->Update();
	}
	std::erase_if(shots_, [](auto shot) {
		return shot->IsDead();
		});
}

void ShotFactory::Draw()
{
	for (auto& shot : shots_) {
		shot->Draw();
	}
}

void ShotFactory::Fire(const Position2& pos, ShotType shotType)
{
	int typeIndex = static_cast<int>(shotType);
	int handle = shotImgs_[typeIndex];
	if (shotType == ShotType::reflect_laser) {
		shots_.push_back(
			std::make_shared<ReflectLaserShot>(handle,pos,Vector2(1.0f,0.0f))
		);
		shots_.push_back(
			std::make_shared<ReflectLaserShot>(handle, pos, Vector2(1.0f, -1.0f).Normalized())
		);
		shots_.push_back(
			std::make_shared<ReflectLaserShot>(handle, pos, Vector2(1.0f, 1.0f).Normalized())
		);
	}
	else {
		shots_.push_back(createFuncs_[typeIndex](handle, pos));
	}
	
}

ShotFactory::Shots_t& ShotFactory::GetShots()
{
	return shots_;
}
