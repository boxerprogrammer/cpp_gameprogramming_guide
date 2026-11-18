#pragma once
#include"Actor.h"
/// <summary>
/// 弾基底クラス
/// </summary>
class Bullet : public Actor
{
protected:
	Vector2 vel_ = {};//一定のスピードで飛んでいくために必要
	int handle_=-1;
public:
	/// <summary>
	/// 弾基底クラスコンストラクタ
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <param name="pos">発生座標</param>
	/// <param name="radius">派生クラスから与えられる半径</param>
	Bullet(int handle , const Position2& pos, float radius);
	/// <summary>
	/// 当たった時に呼び出されるイベント関数
	/// </summary>
	/// <param name="actor">相手オブジェクト</param>
	virtual void OnHit(const std::shared_ptr<Actor> actor)override;
};

