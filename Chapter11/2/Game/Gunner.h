#pragma once
#include "Enemy.h"
class GameScene;
/// <summary>
/// 砲兵:放物線の弾を撃ってくる
/// </summary>
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
    //考える時間：弾を撃つか、移動するか、ターンするかを即時に決定すると
    //あわただしい動きになるため、行動と行動の間には一定時間を設けるようにする
    int thinkFrame_=0;

    /// <summary>
    /// 方向を決める
    /// </summary>
    void DecideUpdate();

    /// <summary>
    /// 進行方向に段差があるときにちょっとジャンプして下降する
    /// </summary>
    void HopForDescentUpdate();

    /// <summary>
    /// 目の前に障害物があった時にジャンプをしてよける
    /// </summary>
    void JumpUpdate();

    /// <summary>
    /// 決めた方向に進む
    /// </summary>
    void WalkUpdate();
    float AdjustScrollX();
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

