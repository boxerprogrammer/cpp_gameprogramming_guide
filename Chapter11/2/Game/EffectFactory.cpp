#include "EffectFactory.h"
#include"../Application.h"
#include"../FileLoader.h"
#include"Effect.h"
#include"PlayerExplosion.h"
#include"ZakoExplosion.h"
#include<algorithm>//いらなくなったエフェクトを自動で削除する用
#include<cassert>

constexpr wchar_t player_explosion_path[] = L"img/Actor/player_explosion.png";
constexpr wchar_t zako_explosion_path[] = L"img/Actor/zako_explosion.png";

EffectFactory::EffectFactory()
{
	auto loader = Application::GetInstance().GetLoader();
	effectImgs_.push_back(
					loader->LoadGraph(player_explosion_path,
					AssetType::temporary));
	effectImgs_.push_back(
					loader->LoadGraph(zako_explosion_path,
					AssetType::temporary));

	createFuncs_.push_back([this](const Position2& pos, int handle) {
		return std::make_shared<PlayerExplosion>(pos, handle);
		});
	createFuncs_.push_back([this](const Position2& pos, int handle) {
		return std::make_shared<ZakoExplosion>(pos, handle);
		});

}

void EffectFactory::Emit(const Position2& pos, EffectType type)
{
	int effectIndex = static_cast<int>(type);
	assert(effectIndex < effectImgs_.size());
	int handle = effectImgs_[effectIndex];
	effects_.push_back(createFuncs_[effectIndex](pos, handle));
}

void EffectFactory::Update()
{
	//すべてのEffectオブジェクトのUpdate()関数を呼ぶ
	for (auto effect : effects_) {
		if (effect->IsDead()) {//もし死んでる状態ならUpdateしない
			continue;
		}
		effect->Update();
	}
	//もしすでに役目を終えてるエフェクトがあればリストから削除します
	std::erase_if(effects_, 
		[](const auto& effect) {
			return effect->IsDead();//削除条件
		});
}

void EffectFactory::Draw()
{
	for (auto effect : effects_) {
		if (effect->IsDead()) {
			continue;
		}
		effect->Draw();
	}
}
