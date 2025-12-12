#pragma once
#include "Enemy.h"
class PataPata :
    public Enemy
{
private:
	int handle_;//画像ハンドル(Factoryからもらったもの)
	int frame_;//経過フレーム
	int life_=2;//2発当てないと死なない
	float sign_ = 1.0f;
public:
	/// <summary>
	/// 雑魚のコンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="pos">初期座標</param>
	PataPata(int handle,
		std::shared_ptr<Player> player,
		std::shared_ptr<BulletFactory> bf,
		std::shared_ptr<EffectFactory> ef,
		const Position2& pos);
	/// <summary>
	/// 雑魚の座標等更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 雑魚の表示
	/// </summary>
	void Draw()override;
	/// <summary>
	/// 当たった時の反応
	/// </summary>
	/// <param name="actor">当たった相手</param>
	void OnHit(const Actor& actor)override;
};

