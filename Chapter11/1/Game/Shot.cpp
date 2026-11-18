#include "Shot.h"
#include<DxLib.h>
#include"../Application.h"
#include"../FileLoader.h"
#include"Collision.h"

constexpr float shot_speed = 8.0f;
constexpr int shot_img_width = 19;
constexpr int shot_img_height = 6;
constexpr float shot_img_scale = 2.0f;
constexpr int anim_interval = 5;//アニメーションの切り替えフレーム
constexpr int out_screen_margin = 20;
Shot::Shot() :Actor({},10.0f),
	vel_{shot_speed,0.0f}
{
	auto loader = Application::GetInstance().GetLoader();
	handle_=loader->LoadGraph(L"img/Actor/shot.png",AssetType::temporary);
	isDead_ = true;
}

void Shot::Init()
{
	CollisionSet();
}

void Shot::Update()
{
	auto& pos = coll_->GetCircle().pos;
	if (IsOutScreen(pos)) {
		isDead_ = true;
	}
	if (isDead_)return;
	pos += vel_;
	++frame_;
}

void Shot::Draw()
{
	if (isDead_)return;
	auto& pos = coll_->GetCircle().pos;
	int idx = (frame_ / anim_interval)%2;//5フレームごとに0,1,0,1を繰り返す
	DrawRectRotaGraph(
		pos.x, pos.y,//表示する位置(画像の中心)
		idx*shot_img_width, 0,//切り取り位置
		shot_img_width, shot_img_height,
		shot_img_scale, 0.0f,
		handle_, true);
}

void Shot::Fire(const Position2& pos)
{
	SetPosition(pos);
	isDead_ = false;
}

void Shot::OnHit(std::shared_ptr<Actor>)
{
	isDead_ = true;
}
