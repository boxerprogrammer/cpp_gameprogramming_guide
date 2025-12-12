#pragma once
#include<memory>
#include"Actor.h"
#include"../Geometry.h"

//プレイヤー
class Player;
//弾生産工場
class BulletFactory;
class EffectFactory;
/// <summary>
/// 敵基底クラス
/// </summary>
class Enemy : public Actor
{
protected:
	Circle circle_;//当たり判定用
	bool isDead_ = false;//死亡フラグ
	//プレイヤーを持っておく(自機狙い弾等を出すため)
	std::shared_ptr<Player> player_;
	//弾をBulletFactoryという弾工場に管理させるために
	//BulletFactoryのオブジェクトを内部に持っておく
	std::shared_ptr<BulletFactory> bulletFactory_;
	std::shared_ptr<EffectFactory> effectFactory_;
public:
	/// <summary>
	/// 敵基底クラスのコンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="bf">弾生産工場のポインタ</param>
	/// <param name="ef">弾生産工場のポインタ</param>
	/// <param name="pos">敵の初期座標</param>
	/// <param name="r">敵の半径</param>
	Enemy(std::shared_ptr<Player> player,//プレイヤーのポインタ
					std::shared_ptr<BulletFactory> bf,//弾生産工場のポインタ
					std::shared_ptr<EffectFactory> ef,//エフェクト生産工場のポインタ
					const Position2& pos,//敵の初期座標
					float r);//敵の当たり判定の大きさ
	const Circle& GetCollision()const;
	bool IsDead()const;
	//他のオブジェクトに当たった時に呼び出されます
	virtual void OnHit(const Actor& actor) {};
	virtual void OnDead();

	/// <summary>
	/// 座標やパラメータの更新
	/// </summary>
	virtual void Update()=0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()=0;

	virtual ~Enemy() {};//←基底クラスのデストラクタはvirtualにしておくこと
};

