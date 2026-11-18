#pragma once
#include"../Geometry.h"
#include<memory>
#include<list>
#include<vector>
#include<functional>

enum class BulletType {
	normal,//通常弾
	cannon_ball,//放物線を描く弾
};

class Bullet;
class EffectFactory;
/// <summary>
/// 弾生産工場
/// </summary>
class BulletFactory
{
private:

	std::weak_ptr<EffectFactory> effectFactory_;
	std::vector<int> bulletImgs_;
	std::list<std::shared_ptr<Bullet>> bullets_;
	using CreateLambda_t = 
		std::function<std::shared_ptr<Bullet>(int, const Position2&, const Vector2&)>;
	using CreateFuncs_t = std::vector<CreateLambda_t>;
	CreateFuncs_t createFuncs_;//弾生成関数(ラムダ式)
public:
	BulletFactory(std::weak_ptr<EffectFactory> ef);
	void Update();
	void Draw();
	void Create(const Position2& pos, const Vector2& vel, BulletType type);
	std::list<std::shared_ptr<Bullet>>& GetBullets();
};

