#include "Boss.h"
#include<DxLib.h>
#include<cassert>
#include"../Application.h"
#include"../Scene/GameScene.h"
#include"Player.h"
#include"BulletFactory.h"
#include"EffectFactory.h"
constexpr float enemy_scale = 1.0f;
constexpr float boss_center_x = 240.0f;
constexpr float boss_center_y = 150.0f;
constexpr float boss_bullet_offset_x = 50.0f;
constexpr float boss_bullet_offset_y = 50.0f;
constexpr float boss_bullet_speed = 4.0f;
constexpr float boss_bullet_angle = DX_PI_F / 6.0f;
constexpr int boss_bullet_interval = 20;
constexpr int nway_increase_interval = 60;
constexpr int appear_frame = 60;
constexpr int max_life = 100;
constexpr int bullet_pattern_num = 6;
constexpr int change_pattern_life = max_life / bullet_pattern_num;

void Boss::AppearUpdate()
{
	pos_ += vel_;
	++frame_;
	if (frame_ >= appear_frame) {
		update_ = &Boss::NormalUpdate;
		frame_ = 0;
	}
}
void Boss::SimpleBulletsUpdate()
{
	MovePosition();
	if (++frame_ % boss_bullet_interval == 0) {
		bulletFactory_->Create(shared_from_this(), pos_, Vector2(0.0f, boss_bullet_speed));
	}
	if (life_<=max_life-change_pattern_life*2)
	{
		frame_ = 0;
		update_ = &Boss::NWayBulletsUpdate;
		return;
	}
}
void Boss::MovePosition()
{
	auto offset = Vector2(cosf(angle_) * radius_, sinf(angle_) * radius_);
	pos_ = Vector2(boss_center_x, boss_center_y) + offset;
	circle_.pos = pos_;
	angle_ += DX_PI_F / 180.0f;
}
void Boss::NWayBulletsUpdate()
{
	MovePosition();
	auto startLife = max_life - change_pattern_life * 2;
	if(++frame_ % boss_bullet_interval == 0){
		ShootNWayBullets((2+startLife-life_) / 2, boss_bullet_angle);
	}
	if (life_<=max_life-change_pattern_life*3){
		frame_ = 0;
		update_ = &Boss::RadialBulletsUpdate;
		return;
	}
}
void Boss::RadialBulletsUpdate()
{
	MovePosition();
	auto startLife = max_life - change_pattern_life * 3;
	//円形弾幕(放射状に弾を吐き出す)
	float rad = DegreeToRadian(static_cast<float>(++frame_));
	if(++frame_ % boss_bullet_interval == 0){
		ShootRadialBullets(8 + (startLife - life_) / 2, rad);
	}
	if(life_<=max_life-change_pattern_life*4){
		frame_ = 0;
		update_ = &Boss::ScatteredBulletsUpdate;
		return;
	}
}
void Boss::ScatteredBulletsUpdate()
{
	MovePosition();
	constexpr int bullet_num = 32;
	float diffAngle = DX_TWO_PI_F / static_cast<float>(bullet_num);
	float angle = 0.0f;
	if(++frame_%boss_bullet_interval==0){
		//バラマキ弾を吐き出す
		for(int i=0;i<bullet_num;++i){
			//方向および速度をランダムに決める
			//方向に関してはランダムなもののあまりに偏りが出ないように
			//角度を360度の範囲で均等に分割して、そこからランダムに角度を付加する
			float rndAngle = angle + DegreeToRadian(static_cast<float>(rand() % (360/bullet_num)));
			constexpr int regular_speed = static_cast<int>(boss_bullet_speed * 2.0f);
			float speed = static_cast<float>(rand() % regular_speed + regular_speed/4);
			bulletFactory_->Create(shared_from_this(), pos_, Vector2(cosf(rndAngle), sinf(rndAngle)) * speed);
			angle += diffAngle;
		}
	}
	if(life_<=max_life-change_pattern_life*5){
		frame_ = 0;
		update_ = &Boss::RainyBulletsUpdate;
		return;
	}
	
}
void Boss::RainyBulletsUpdate()
{
	MovePosition();
	//雨のように下に加速する弾を出す
	if (++frame_ % boss_bullet_interval == 0) {
		constexpr int bullet_num = 20;
		float diffAngle = DX_PI_F / static_cast<float>(bullet_num);
		float angle = 0.0f;
		for (int i = 0; i < bullet_num; ++i) {
			auto acc = Vector2(0.0f, 0.1f);
			auto vel = Vector2(cos(angle),-sin(angle))*boss_bullet_speed;
			bulletFactory_->Create(shared_from_this(), pos_, vel, acc);
			angle += diffAngle;
		}
	}
}
void Boss::NormalUpdate()
{
	MovePosition();
	if (life_<=max_life-change_pattern_life*1)
	{
		frame_ = 0;
		update_ = &Boss::SimpleBulletsUpdate;
		return;
	}
	//if(life_ <= 0) {
	//	update_ = &Boss::DyingUpdate;
	//	draw_ = &Boss::DyingDraw;
	//	gameScene_.OnExitBoss();
	//}
}
void Boss::DyingUpdate()
{
	if (isDead_)return;
	int x = rand() % 200-100;
	int y = rand() % 200-100;
	if (++frame_ % 5 == 0) {
		effectFactory_->Create(pos_ + Vector2(static_cast<float>(x), static_cast<float>(y)), EffectType::explosion);
	}
	if(frame_>=120){
		isDead_ = true;
		OnDead();
		gameScene_.OnExitBoss();
		return;
	}
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
void Boss::ShootNWayBullets(int wayNum, float angle)
{
	assert(wayNum > 0);
	//自機狙いベクトルを作っておく
	auto vec = (player_->GetPos() - pos_).Normalized();
	if (wayNum == 1) {//1-wayのときは、角度をそのまま使う
		bulletFactory_->Create(shared_from_this(), pos_, vec*boss_bullet_speed);
		return;
	}
	float oneAngle = angle / static_cast<float>(wayNum - 1);//1つの弾の角度
	//最初の角度(中心が自機狙い、かつnwayが奇数の時は真ん中が自機狙い、偶数の時は自機外しになるようにする)
	float startAngle = atan2f(vec.y, vec.x) - 
		oneAngle * (static_cast<float>(wayNum / 2)-static_cast<float>((wayNum+1)%2)*0.5f);
	float bulletAngle = startAngle;
	for (int i = 0; i < wayNum; ++i) {
		bulletFactory_->Create(shared_from_this(), //thisをshared_ptrに変換して渡す
			pos_, //弾の初期位置
			Vector2(cosf(bulletAngle), sinf(bulletAngle)) * boss_bullet_speed);
		bulletAngle += oneAngle;
	}
}
void Boss::ShootRadialBullets(int wayNum, float angle){
	assert(wayNum > 4);
	float oneAngle = 2.0f * DX_PI_F / static_cast<float>(wayNum);
	for (int i = 0; i < wayNum; ++i) {
		bulletFactory_->Create(shared_from_this(),
			pos_,
			Vector2(cosf(angle), sinf(angle)) * boss_bullet_speed);
		angle += oneAngle;
	}
}
Boss::Boss(GameScene& gameScene, int handle, std::shared_ptr<Player> player, std::shared_ptr<BulletFactory> bulletFactory, std::shared_ptr<EffectFactory> effectFactory, Vector2 pos):
	Enemy(player, bulletFactory, effectFactory, pos,50),handle_(handle),gameScene_(gameScene)
{
	life_ = 100;
	frame_ = 0;
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

void Boss::OnHit(const Actor& actor)
{
	if (--life_ <= 0) {
		update_ = &Boss::DyingUpdate;
		draw_ = &Boss::DyingDraw;
		frame_ = 0;
	}
}

void Boss::Damage(int damage)
{
	life_ -= damage;
	if (life_ <= 0) {
		update_ = &Boss::DyingUpdate;
		draw_ = &Boss::DyingDraw;
		frame_ = 0;
	}
}
