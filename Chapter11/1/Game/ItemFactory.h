#pragma once
#include"../Geometry.h"
#include"Item.h"
#include<memory>
#include<list>
#include<vector>

class GameScene;
/// <summary>
/// 画面上のアイテムを管理するクラス
/// </summary>
class ItemFactory
{
private:
	using Items_t = std::list<std::shared_ptr<Item>>;
	Items_t items_;//アイテムの集合体
	std::weak_ptr<GameScene> gameScene_;//ゲームシーン
	std::vector<int> itemImgs_;//アイテム画像
public:
	ItemFactory(std::weak_ptr<GameScene> gs);
	void Update();
	void Draw();
	Items_t& GetItems();
	/// <summary>
	/// アイテムを生成する
	/// </summary>
	/// <param name="pos">アイテムの座標</param>
	/// <param name="type">アイテム種別</param>
	void Create(const Position2& pos, ItemType type);
};

