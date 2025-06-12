#pragma once
#include "Actor.h"
/// <summary>
/// ������I�u�W�F�N�g
/// </summary>
class Ball :
    public Actor
{
public:
	Ball(const Position2& pos);
	virtual void OnHit(const Actor* targetAct)override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual bool IsDead()const override;
	virtual ActorType GetType()const override;
};

