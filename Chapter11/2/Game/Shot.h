#pragma once
#include "Actor.h"
//プレイヤーの弾基底クラス
class Shot : public Actor
{
protected:
	int handle_=-1;
	Vector2 vel_ = {};//弾の速度
	int frame_ = 0;
public:
	Shot(int handle,const Position2& pos,float radius);
	virtual void Init();
};

