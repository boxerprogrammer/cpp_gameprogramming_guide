#include "Player.h"
#include"Enemy.h"
#include"../Input.h"
#include"../Scene/GameScene.h"
#include"../ResourceManager.h"
#include<DxLib.h>
#include<cassert>

constexpr float player_speed = 4.0f;//プレイヤーの移動速度
constexpr float player_shot_speed = 8.0f;//プレイヤーの弾速度
constexpr float player_scale = 2.0f;//プレイヤー画像のスケーリング
constexpr int player_cut_w = 16;
constexpr int player_cut_h = 24;

void Player::NormalUpdate(Input& input) {
	Vector2 vel = {};//プレイヤーの速度ベクトル
	if (input.IsPressed("up")) {
		vel.y = -1.0f;
	}
	if (input.IsPressed("right")) {
		vel.x = 1.0f;
	}
	if (input.IsPressed("down")) {
		vel.y = 1.0f;
	}
	if (input.IsPressed("left")) {
		vel.x = -1.0f;
	}
	circle_.pos += vel.Normalized() * player_speed;
	pos_ = circle_.pos;
}
void Player::JustDieUpdate(Input& input)
{
	update_ = &Player::DyingUpdate;
}
void Player::DyingUpdate(Input& input)
{
	if (--dyingFrame_ <= 0) {
		update_ = &Player::NormalUpdate;
	}
}

Player::Player(GameScene& gameScene):
	gameScene_(gameScene),
	circle_{ {320,400},10.0f }
{
	update_ = &Player::NormalUpdate;
	pos_ = circle_.pos;
	actorType_ = ActorType::player;
	//使用する画像をロード
	handle_ = mylib::LoadTexture(L"img/game/player.png");
	assert(handle_ >= 0);

}


Player::~Player()
{
	DeleteGraph(handle_);
}

const Circle& 
Player::GetCollision()const {
	return circle_;
}

void Player::Update()
{
	
}

void Player::Update(Input& input)
{
	(this->*update_)(input);
	Update();
}

void Player::Draw()
{
	if (update_!=&Player::DyingUpdate) {//死んでないときだけ表示
		int playerImgIdx = (frame_ / 8) % 2;
		DrawRectRotaGraph(pos_.x, pos_.y,//プレイヤーの表示座標
			player_cut_w * 2, player_cut_h * playerImgIdx,//切り取りの左上
			player_cut_w, player_cut_h,//切り取りの幅高
			player_scale, 0.0f, handle_, true);
	}
}

void Player::OnHit(const Actor& actor)
{
	if (actor.GetActorType() == ActorType::enemy || actor.GetActorType() == ActorType::enemy_bullet) {
		dyingFrame_ = 60;
		update_ = &Player::JustDieUpdate;
		gameScene_.DoHitStop();
	}
}
