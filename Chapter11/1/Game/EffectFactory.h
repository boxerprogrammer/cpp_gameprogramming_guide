#pragma once
#include<list>//次から次へ生成、削除が行われるためlist
#include<vector>//エフェクト画像ハンドルを事前にとっておくため
#include<memory>
#include<functional>
#include"../Geometry.h"
class Effect;

enum class EffectType {
	player_explosion,//プレイヤー爆発:0
	zako_explosion,//ザコ爆発:1
};

class EffectFactory
{
private:
	using CreateFuncs_t = 
		std::vector<std::function<std::shared_ptr<Effect>(const Position2& pos, int)>>;
	CreateFuncs_t createFuncs_;
	std::vector<int> effectImgs_;//エフェクト画像ハンドル
	std::list < std::shared_ptr<Effect>> effects_;//エフェクトオブジェクト
public:
	EffectFactory();
	/// <summary>
	/// エフェクトを生成する
	/// </summary>
	/// <param name="pos">発生座標</param>
	/// <param name="type">エフェクト種別</param>
	void Emit(const Position2& pos, EffectType type);

	void Update();
	void Draw();
};

