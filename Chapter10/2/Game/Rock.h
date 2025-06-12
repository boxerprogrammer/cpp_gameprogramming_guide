#pragma once
#include "Actor.h"

/// <summary>
/// 押せないオブジェクト(障害物)
/// </summary>
class Rock :
    public Actor
{
public:
	Rock(const Position2& pos);
	virtual void OnHit(const Actor* targetAct)override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual bool IsDead()const override;
	virtual ActorType GetType() const override;
};

