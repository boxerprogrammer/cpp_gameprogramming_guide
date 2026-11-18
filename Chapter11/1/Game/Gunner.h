#pragma once
#include "Enemy.h"
class GameScene;
class Gunner :
    public Enemy
{
    std::weak_ptr<GameScene> gameScene_;
    int initScroll_ = 0;
    Position2 basicPos_ = {};
    Vector2 vel_ = {};
    int GetScroll();

    //敵が考えて動いているように見せるためにステートマシンを作ります
    using Update_t = void(Gunner::*)();
    Update_t update_;

    int thinkFrame_=0;//考える時間

    /// <summary>
    /// 方向を決める
    /// </summary>
    void DecideUpdate();
    /// <summary>
    /// 決めた方向に進む
    /// </summary>
    void WalkUpdate();
    /// <summary>
    /// 何もしない時間
    /// </summary>
    void IdleUpdate();
    /// <summary>
    /// 攻撃
    /// </summary>
    void AttackUpdate();

    //進むべき方向を計算する
    void CalculateVelocity();
public:
    Gunner(int handle, const Position2& pos, std::weak_ptr<Player> player, std::shared_ptr<EffectFactory> ef, std::shared_ptr<BulletFactory> bf, std::weak_ptr<GameScene> gs);
    
    void Update()override;
    void Draw()override;

};

