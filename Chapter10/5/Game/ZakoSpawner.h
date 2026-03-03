#pragma once
#include "Enemy.h"
class EnemyFactory;
class ZakoSpawner : public Enemy
{
	int frame_ = 0;
	EnemyFactory& enemyFactory_;
public:
	ZakoSpawner(EnemyFactory& enemFac,const Position2& pos);
	/// <summary>
	/// 雑魚の座標等更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 雑魚の表示
	/// </summary>
	void Draw()override;
};

