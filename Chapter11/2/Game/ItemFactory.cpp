#include "ItemFactory.h"
#include<algorithm>
#include"../Application.h"
#include"../FileLoader.h"
ItemFactory::ItemFactory(std::weak_ptr<GameScene> gs):gameScene_(gs)
{
	//enum class ItemType {
	//	none,//
	//	speedup,//スピードアップアイテム
	//	force,//フォース
	//	missile,//ミサイル
	//	laser//レーザー
	//};
	//アイテム素材のロード
	auto loader = Application::GetInstance().GetLoader();
	itemImgs_.push_back(-1);
	itemImgs_.push_back(loader->LoadGraph(L"img/item/item_speed.png", AssetType::temporary));
	itemImgs_.push_back(loader->LoadGraph(L"img/item/item_force.png", AssetType::temporary));
	itemImgs_.push_back(loader->LoadGraph(L"img/item/item_missile.png", AssetType::temporary));
	itemImgs_.push_back(loader->LoadGraph(L"img/item/item_3way_laser.png", AssetType::temporary));
}

void ItemFactory::Update()
{
	for (auto item : items_) {
		item->Update();
	}
	//IsDeadのものは削除する
	std::erase_if(items_,
		[](const auto item) {
			return item->IsDead();
		});
}

void ItemFactory::Draw()
{
	//全アイテムを描画する
	for (auto item : items_) {
		item->Draw();
	}
}

ItemFactory::Items_t& ItemFactory::GetItems()
{
	return items_;
}

void ItemFactory::Create(const Position2& pos, ItemType type)
{
	int typeIdx = static_cast<int>(type);//enumのアイテム種別をインデックスに変換
	int handle = itemImgs_[typeIdx];
	//アイテムを生成し、管理リストに追加する
	items_.push_back(std::make_shared<Item>(handle,pos, type, gameScene_));
}
