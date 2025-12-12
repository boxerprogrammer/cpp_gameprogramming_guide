#pragma once
#include"../Geometry.h"
/// <summary>
/// 配置可能なゲームオブジェクトの基底クラス
/// </summary>
class Actor
{
protected:
	Position2 pos_;
public:
	virtual void Update()=0;
	virtual void Draw()=0;
	virtual void OnHit(const Actor& actor) = 0;
	virtual ~Actor() {};
	const Position2& GetPos()const;
};

