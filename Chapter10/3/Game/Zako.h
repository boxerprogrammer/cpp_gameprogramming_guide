#pragma once
#include "Enemy.h"
/// <summary>
/// 雑魚クラス
/// </summary>
class Zako : public Enemy
{
private:
	int handle_;//画像ハンドル(Factoryからもらったもの)
	int frame_;//経過フレーム

	using UpdateFunc_t = void (Zako::*)();
	UpdateFunc_t update_;

	void ForwardUpdate();
	void BackUpdate();

public:
	/// <summary>
	/// 雑魚のコンストラクタ
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="pos">初期座標</param>
	Zako(int handle,
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

