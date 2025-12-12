#pragma once

#include"Actor.h"
class Input;
class Enemy;
class Player : public Actor
{
public:
	Player();
	void Update()override;
	void Draw()override;
	void Update(Input&);
	void OnHit(const Actor& actor)override {};
};

