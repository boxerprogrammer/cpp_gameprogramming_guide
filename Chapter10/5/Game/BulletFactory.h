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
/// ’e¶YHê
/// </summary>
class BulletFactory
{
private:
	int handle_ = -1;
	std::list<std::shared_ptr<Bullet>> bullets_;
public:
	BulletFactory();
	/// <summary>
	/// ’e‚ÌƒŠƒXƒg‚ğ•Ô‚·
	/// </summary>
	/// <returns>’e‚ÌƒŠƒXƒg</returns>
	std::list<std::shared_ptr<Bullet>>& GetBullets() { return bullets_; }
	/// <summary>
	/// ’e‚ğ¶¬‚·‚é
	/// </summary>
	/// <param name="owner">’e‚ÌŠ—LÒ</param>
	/// <param name="pos">’e‚Ì‰ŠúˆÊ’u</param>
	/// <param name="vel">’e‚Ì‰Šú‘¬“x</param>
	void Create(std::shared_ptr<Actor> owner, const Position2& pos,const Vector2& vel);
	/// <summary>
	/// ’e‚ğ‘S‚Äíœ‚·‚é
	/// </summary>
	void Clear();
	/// <summary>
	/// ’e‚Ìó‘Ô‚ğ‚·‚×‚ÄXV
	/// </summary>
	void Update();
	/// <summary>
	/// ’e‚ğ‚·‚×‚Ä•`‰æ
	/// </summary>
	void Draw();
};

