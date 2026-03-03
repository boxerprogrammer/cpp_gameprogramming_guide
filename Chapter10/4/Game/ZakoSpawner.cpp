#include "ZakoSpawner.h"
#include"EnemyFactory.h"
#include"../Application.h"
#include<DxLib.h>

ZakoSpawner::ZakoSpawner(EnemyFactory& enemFac, const Position2& pos):
	Enemy(nullptr,nullptr, nullptr, pos, 0.0f),
	enemyFactory_(enemFac)
{
}

void ZakoSpawner::Update()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	if (frame_%20  == 0) {
		Position2 pos = { -60+GetRand(60)-30,0};
		auto zako = enemyFactory_.Create(0, 0, EnemyType::zako);
		zako->SetPosition(pos);
		auto zako2 = enemyFactory_.Create(0, 0, EnemyType::zako);
		pos.x = wsize.w + 60+ GetRand(60) - 30;
		zako2->SetPosition(pos);
	}
	++frame_;
	if (frame_ >= 600) {
		isDead_ = true;
	}
}

void ZakoSpawner::Draw()
{
}
