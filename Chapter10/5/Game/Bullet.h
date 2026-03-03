#pragma once
#include "Actor.h"
/// <summary>
/// 弾オブジェクト(仮)
/// </summary>
class Bullet : public Actor
{
public:
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnHit(const Actor& actor) {};
};

