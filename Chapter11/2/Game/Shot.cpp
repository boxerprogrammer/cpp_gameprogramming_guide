#include "Shot.h"
#include<DxLib.h>

#include"Collision.h"


Shot::Shot(int handle,const Position2& pos, float radius) :Actor(pos,radius),
	handle_(handle)
{
	
	isDead_ = false;
}

void Shot::Init()
{
	CollisionSet();
}


