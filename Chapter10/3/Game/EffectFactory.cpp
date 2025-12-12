#include "EffectFactory.h"
#include"Effect.h"
#include"Damage.h"
#include"Explosion.h"
#include"EnemyDeadExplosion.h"
#include<algorithm>
#include<DxLib.h>
#include<cassert>

EffectFactory::EffectFactory()
{
	int handle = -1;
	handles_.push_back(handle);
	handle = LoadGraph(L"resource/img/game/damage.png");
	assert(handle >= 0);
	handles_.push_back(handle);
	handle = LoadGraph(L"resource/img/game/explosion.png");
	assert(handle >= 0);
	handles_.push_back(handle);
	handle = LoadGraph(L"resource/img/game/enemy_dead_1.png");
	assert(handle >= 0);
	handles_.push_back(handle);
}

void EffectFactory::Create(const Position2& pos, EffectType type)
{
	switch (type) {
	case EffectType::damage:
		effects_.push_back(std::make_shared<Damage>(handles_[(int)type], pos));
		break;
	case EffectType::explosion:
		effects_.push_back(std::make_shared<Explosion>(handles_[(int)type], pos));
		break;
	case EffectType::enemy_dead_1:
		effects_.push_back(std::make_shared<EnemyDeadExplosion>(handles_[(int)type], pos));
		break;
	default:
		break;
	}
}


void 
EffectFactory::Update() {
	for (auto& effect : effects_) {
		effect->Update();
	}
	effects_.remove_if([](std::shared_ptr<Effect> effect) {
		return effect->IsDead();
		});
}
void 
EffectFactory::Draw() {
	for (auto& effect : effects_) {
		effect->Draw();
	}
}