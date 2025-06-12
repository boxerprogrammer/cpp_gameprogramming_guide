#pragma once
#include<vector>
#include<memory>
#include"Geometry.h"
class Collider;
class CollisionManager
{
private:
	std::vector<std::shared_ptr<Collider>> colliders_;
	bool IsHit(const Circle& cA, const Circle& cB)const;
public:
	void CheckCollision();
	void Update();
	void Sweep();
	void AddCollider(std::shared_ptr<Collider> coll);
};

