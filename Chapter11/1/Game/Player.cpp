#include "Player.h"
#include"../Application.h"
#include"../FileLoader.h"
#include"../Input.h"
#include"EffectFactory.h"
#include<DxLib.h>
#include<algorithm>
constexpr float player_speed = 4.0f;
constexpr float player_scale = 2.0f;
constexpr int player_img_width = 26;
constexpr int player_img_height = 21;

constexpr int neutral_img_idx = 0;//通常の画像インデックス
constexpr int downer_img_idx = 1;//下降するときのインデックス
constexpr int upper_img_idx = 2;//上昇するときのインデックス

constexpr float player_hit_radius = 10.0f;//当たり判定の大きさ

constexpr int resurrecting_interval = 120;//復活までの時間

void Player::NormalUpdate()
{
}

void Player::ResurrectingUpdate()
{
	if (--resurrectFrame_ <= 0) {
		update_ = &Player::NormalUpdate;
	}
	
}


Player::Player(std::shared_ptr<EffectFactory> ef) :
	Actor({},player_hit_radius),
	effectFactory_(ef)
{
	type_ = ActorType::player;
	auto& pos = coll_->GetCircle().pos;
	const auto& size = Application::GetInstance().GetWindowSize();
	pos = { size.w*0.5f,size.h*0.5f};
	handle_ = Application::GetInstance().GetLoader()->LoadGraph(L"img/Actor/player.png",AssetType::temporary);
	update_ = &Player::NormalUpdate;
}

bool Player::IsResurrecting() const
{
	return update_ == &Player::ResurrectingUpdate;
}

void Player::Update(Input& input)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	Vector2 vel = {};//速度ベクトル
	imgIndex_ = neutral_img_idx;//何もしてない時の画像インデックス
	if (input.IsPressed("left")) {
		vel.x = -1.0f;
	}
	if (input.IsPressed("right")) {
		vel.x = 1.0f;
	}
	if (input.IsPressed("up")) {
		imgIndex_ = upper_img_idx;//上向き画像
		vel.y = -1.0f;
	}
	if (input.IsPressed("down")) {
		imgIndex_ = downer_img_idx;//下向き画像
		vel.y = 1.0f;
	}
	auto& pos = coll_->GetCircle().pos;//あらかじめアクターの座標の参照を取得
	vel.Normalize();//正規化で大きさを１にする(√2になってるかもしれない)
	vel *= player_speed;//正規化したベクトルに対してスピードを乗算
	pos+=vel;//正規化したうえでスピードを適用

	//画面端を超えないように補正
	AdjustPlayerPosition(pos, wsize);

	Update();
}

void Player::AdjustPlayerPosition(Position2& pos, const Size& wsize)
{
	//DrawRotaGraphの座標は画像の中心になるため、半分めり込んでしまう
	//これをプレイヤー画像の半分を計算し、それも含めて補正する
	auto halfX = (player_img_width * player_scale) * 0.5f;
	auto halfY = (player_img_height * player_scale) * 0.5f;
	//横幅制限
	pos.x = std::clamp(pos.x, //値
		0.0f + halfX, //最小値
		static_cast<float>(wsize.w) - halfX);//最大値
	//縦幅制限
	pos.y = std::clamp(pos.y, 0.0f + halfY, static_cast<float>(wsize.h) - halfY);
}

void Player::Update()
{
	(this->*update_)();
}

void Player::Draw()
{
	auto& circle = coll_->GetCircle();
	auto& pos = circle.pos;
	//もしresurrectFrame_が0より大きい場合は点滅表示をさせる
	if ((resurrectFrame_ / 5) % 2 == 0) {
		//画像の一部を表示する(拡大縮小回転付き)
		DrawRectRotaGraph(pos.x, pos.y,//プレイヤーの座標
			player_img_width * imgIndex_, 0,//切り取り左,上 
			player_img_width, player_img_height, //切り取り幅,高さ
			player_scale, 0.0f,//画像の拡大率,回転角度
			handle_, true);//ハンドル,透過
	}
	
	DrawCircleAA(pos.x, pos.y, circle.radius, 16, 0xffffff, false, 3.0f);
}

void Player::OnHit(std::shared_ptr<Actor> actor)
{
	if (IsDead())return;
	if (IsResurrecting())return;

	if (!actor) {//相手のアクターが空アクターだったら地形に当たったとみなします
		KillPlayer();
		return;
	}

	auto type = actor->GetType();
	if (type == ActorType::enemy || type==ActorType::bullet) {
		KillPlayer();
		return;
	}
}

void Player::KillPlayer()
{
	//プレイヤー爆発エフェクトを発生させる
	effectFactory_->Emit(GetPosition(), EffectType::player_explosion);
	//一度死ぬので復活状態へ移行する
	update_ = &Player::ResurrectingUpdate;//復活中
	//復活までに要する時間を入れておく
	resurrectFrame_ = resurrecting_interval;
}
