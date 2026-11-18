#pragma once
#include "Actor.h"
class Effect : public Actor
{
protected:
	int frame_=0;
	int maxFrame_=0;
	int handle_=-1;
public:
	Effect(const Position2& pos,int maxFrame,int handle);
	virtual void Update();
	virtual void Draw() = 0;
	void OnHit(const std::shared_ptr<Actor> actor)override {};
};

