#include "Force.h"
#include"Player.h"
#include<DxLib.h>

constexpr float force_radius = 16.0f;
constexpr int force_img_width = 64;
constexpr int force_img_height = 64;
constexpr int force_img_scale = 1.0f;
constexpr int force_anim_num = 3;
constexpr int force_anim_interval = 5;

constexpr float player_scale = 2.0f;
constexpr int player_img_width = 26;


Force::Force(int handle, std::weak_ptr<Player> player, std::weak_ptr<GameScene> gs) :
	Actor({}, force_radius),
	handle_(handle),
	player_(player),
	gameScene_(gs)

{
	isDead_ = true;
	type_ = ActorType::force;
}

void Force::Update()
{
	if (isDead_)return;
	if (!player_.expired()) {
		auto& pos = GetPosition();
		pos = player_.lock()->GetPosition() + 
			Vector2((player_img_width * player_scale + force_radius) / 2.0f, 
					0.0f);//プレイヤーの前方にくっつける
	}
	++frame_;
}

void Force::Draw()
{
	if (isDead_)return;
	auto& pos = GetPosition();
	int idx = (frame_ / force_anim_interval) % force_anim_num;
	DrawRectRotaGraph(pos.x, pos.y,
		idx * force_img_width, 0,
		force_img_width, force_img_height,
		force_img_scale, 0.0f,
		handle_, true);
	GetCollision()->Draw();
}

void Force::OnHit(const std::shared_ptr<Actor> actor)
{

}

void Force::Run()
{
	isDead_ = false;
}
