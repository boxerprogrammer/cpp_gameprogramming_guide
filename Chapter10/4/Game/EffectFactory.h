#pragma once
#include"../Geometry.h"
#include<memory>
#include<list>
#include<vector>
class Effect;
enum class EffectType {
	none,
	damage,
	explosion,
	enemy_dead_1,//敵死亡エフェクト①
};
/// <summary>
/// エフェクトを生成、管理するだけのクラス
/// </summary>
class EffectFactory
{
private:
	std::vector<int> handles_;
	std::list<std::shared_ptr<Effect>> effects_;
public:
	EffectFactory();
	void Create(const Position2& pos,EffectType type);
	void Update();
	void Draw();
};

