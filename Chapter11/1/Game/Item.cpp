#include "Item.h"
#include"../Scene/GameScene.h"
#include"Stage.h"
#include<DxLib.h>
constexpr float item_radius = 32.0f;
constexpr int img_width = 32;
constexpr int img_height = 32;
constexpr int img_scale = 1.0f;


Item::Item(int handle, const Position2& pos, ItemType type,std::weak_ptr<GameScene> gs):
	Actor(pos,item_radius),
	handle_(handle),
	gameScene_(gs)
{
	if (!gameScene_.expired()) {
		initScroll_=gameScene_.lock()->GetStage()->GetCurrentScroll();
	}
	basePos_ = pos;
	type_ = ItemType::none;
}

void Item::Update()
{
	if (!gameScene_.expired()) {
		auto& pos = GetPosition();
		pos.x = basePos_.x - (gameScene_.lock()->GetStage()->GetCurrentScroll() - initScroll_);
		//画面外に出たら削除対象
		if (IsOutScreen(pos)) {
			isDead_ = true;
		}
	}
}

void Item::Draw()
{
	auto& pos = GetPosition();
	DrawRectRotaGraph(pos.x, pos.y,
						0, 0,
						img_width, img_height,
						img_scale, 0.0f,
						handle_, true);
}

void Item::OnHit(const std::shared_ptr<Actor> actor)
{
	if (actor->GetType() == ActorType::player) {
		isDead_ = true;
	}
}

ItemType Item::GetType() const
{
	return type_;
}
