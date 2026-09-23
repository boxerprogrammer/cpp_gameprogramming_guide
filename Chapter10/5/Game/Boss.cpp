#include "Boss.h"
#include<DxLib.h>
#include"../Application.h"
#include"../Scene/GameScene.h"
constexpr float enemy_scale = 1.0f;
constexpr float boss_center_x = 240.0f;
constexpr float boss_center_y = 150.0f;
constexpr float boss_bullet_offset_x = 50.0f;
constexpr float boss_bullet_offset_y = 50.0f;
constexpr float boss_bullet_speed = 4.0f;
constexpr float boss_bullet_angle = DX_PI_F / 6.0f;
constexpr int boss_bullet_interval = 30;
constexpr int appear_frame = 60;
void Boss::AppearUpdate()
{
	pos_ += vel_;
	++frame_;
	if (frame_ >= appear_frame) {
		update_ = &Boss::NormalUpdate;
	}
}
void Boss::SimpleBulletsUpdate()
{

}
void Boss::NWayBulletsUpdate()
{}
void Boss::RadialBulletsUpdate()
{}
void Boss::ScatteredBulletsUpdate()
{}
void Boss::RainyBulletsUpdate()
{}
void Boss::NormalUpdate()
{
	auto offset = Vector2(cosf(angle_) * radius_, sinf(angle_) * radius_);
	pos_ = Vector2(boss_center_x, boss_center_y) + offset;
	circle_.pos = pos_;
	angle_ += DX_PI_F / 180.0f;
	if(life_ <= 0) {
		update_ = &Boss::DyingUpdate;
		draw_ = &Boss::DyingDraw;
		gameScene_.OnExitBoss();
	}
}
void Boss::DyingUpdate()
{
}
void Boss::NormalDraw()
{
	DrawRotaGraph(pos_.x, pos_.y,
					enemy_scale, 0.0f,
					handle_, true);
}
void Boss::DyingDraw()
{
}
Boss::Boss(GameScene& gameScene, int handle, std::shared_ptr<Player> player, std::shared_ptr<BulletFactory> bulletFactory, std::shared_ptr<EffectFactory> effectFactory, Vector2 pos):
	Enemy(player, bulletFactory, effectFactory, pos,50),handle_(handle),gameScene_(gameScene)
{
	life_ = 100;
	update_ = &Boss::AppearUpdate;
	draw_ = &Boss::NormalDraw;
	angle_ = DX_PI_F / 2.0f;
	auto offset = Vector2(cosf(angle_) * radius_, sinf(angle_) * radius_);
	vel_ = (Vector2(boss_center_x, boss_center_y)+offset - pos);
	float speed = vel_.Length() / static_cast<float>(appear_frame);
	vel_ = vel_.Normalized() * speed;
	gameScene_.OnEnterBoss();
	bulletsUpdate_ = &Boss::SimpleBulletsUpdate;
}

void Boss::Update()
{
	(this->*update_)();
}

void Boss::Draw()
{
	(this->*draw_)();
}
