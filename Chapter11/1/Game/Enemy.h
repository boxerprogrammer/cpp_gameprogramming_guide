#pragma once
#include "Actor.h"

class Player;
class EffectFactory;
class BulletFactory;
/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public Actor
{
protected:
    std::shared_ptr<EffectFactory> effectFactory_;//エフェクト発生用
    std::shared_ptr<BulletFactory> bulletFactory_;//弾発生用
    std::weak_ptr<Player> player_;//プレイヤーの弱参照
    int handle_=-1;//敵の画像ハンドル
    int frame_=0;
public:
    Enemy(int handle,const Position2& pos,float radius,std::weak_ptr<Player> player,std::shared_ptr<EffectFactory> ef,std::shared_ptr<BulletFactory> bf);
    virtual void OnHit(std::shared_ptr<Actor>)override;
    void Init();
};

