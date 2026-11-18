#pragma once
#include "Enemy.h"
/// <summary>
/// ザコクラス
/// </summary>
class Zako : public Enemy
{
private:
    Vector2 vel_;//敵の速度
    Position2 initPos_;//初期座標
    float angle_ = 0.0f;//sinカーブさせたいため角度を持たせる

    //敵の行動を複数パターンにしたいため
    //状態遷移ができるように内部にメンバ関数ポインタを持たせている
    //ゲームAI定番の「ステートマシン(FSM)」になっています
    using Update_t = void (Zako::*)();
    Update_t update_;
    void ForwardUpdate();//前進
    void BackUpdate();//後退

public:
    Zako(int handle, const Position2& pos, std::weak_ptr<Player> player, std::shared_ptr<EffectFactory> ef, std::shared_ptr<BulletFactory> bf);
    void Update()override;
    void Draw()override;
};

