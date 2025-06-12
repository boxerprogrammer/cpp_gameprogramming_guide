#pragma once
#include "Actor.h"

/// <summary>
/// �����Ȃ��I�u�W�F�N�g(��Q��)
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

