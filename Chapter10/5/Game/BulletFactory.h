#pragma once
#include<map>
#include<list>

#include"Bullet.h"
#include "../Geometry.h"

enum class BulletType {
	none,
	player,
	enemy,
};

class Actor;

/// <summary>
/// íeê∂éYçHèÍ
/// </summary>
class BulletFactory
{
private:
	int handle_ = -1;
	std::list<std::shared_ptr<Bullet>> bullets_;
public:
	BulletFactory();
	std::list<std::shared_ptr<Bullet>>& GetBullets() { return bullets_; }
	void Create(std::shared_ptr<Actor> owner, const Position2& pos,const Vector2& vel);
	void Clear();
	void Update();
	void Draw();

};

