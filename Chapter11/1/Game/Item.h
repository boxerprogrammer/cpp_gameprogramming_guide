#pragma once
#include "Actor.h"
enum class ItemType {
	none,//
};

class GameScene;

/// <summary>
/// アイテム基底クラス
/// プレイヤーがこれに触れるとパワーアップ等を行う
/// </summary>
/// 基本的にアイテムは自分で動かないので、背景と一緒にスクロールする
class Item : public Actor
{
	int handle_;//画像ハンドル
	ItemType type_;//アイテム種別
	std::weak_ptr<GameScene> gameScene_;//スクロールの取得のため
	int initScroll_ = 0;//スクロール値を覚えておくため
	Position2 basePos_ = {};//最初の座標を覚えておくため
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <param name="pos">座標</param>
	/// <param name="type">アイテム種別</param>
	/// <param name="gs">GameScene</param>
	Item(int handle,const Position2& pos,ItemType type,  std::weak_ptr<GameScene> gs);
	void Update()override;
	void Draw()override;
	void OnHit(const std::shared_ptr<Actor> actor)override;
	/// <summary>
	/// アイテム種別を返す
	/// </summary>
	/// <returns>アイテム種別</returns>
	ItemType GetType()const;
};

