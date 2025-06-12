#include "GameScene.h"
#include<DxLib.h>
#include<cassert>

#include"../Input.h"//入力用
#include"SceneController.h"//シーンの切り替えに使う
#include"OverScene.h"//次のシーン
#include"PauseScene.h"//ポーズシーン
#include"../DrawUtil.h"

namespace {
	constexpr int fade_interval = 60;
	float DegreeToRadian(float degree) {
		//0～360→0～2π
		//0～180→0～π
		//÷180 x π
		return (degree * DX_PI_F) / 180.0f;
	}
}

void GameScene::FadeInUpdate(Input& input)
{
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	++frame_;
	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frame_ = 0;
		return;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}
	if (input.IsPressed("up")) {
		cameraPos_.y += 0.1f;
	}
	if (input.IsPressed("down")) {
		cameraPos_.y -= 0.1f;
	}
	if (input.IsPressed("right")) {
		cameraPos_.x += 0.1f;
	}
	if (input.IsPressed("left")) {
		cameraPos_.x -= 0.1f;
	}
	if(input.IsTriggered("shot")){
		yureFrame_ = 360;
		yureRate_ = 1.0f;
	}
	if (yureFrame_ > 0) {
		yureRate_ *= 0.95f;
		--yureFrame_;
	}
	else {
		yureRate_ = 0.0f;
	}
	
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<OverScene>(controller_));
		return;
	}
}

void GameScene::NormalDraw()
{
	SetDrawScreen(RT_);
	ClearDrawScreen();
	DrawRotaGraph(320, 240, 1.0f, 0.0f, bgH_, true);
	DrawString(10, 10, L"Game Scene", 0xffffff);
	SetDrawScreen(DX_SCREEN_BACK);
	int x = ((yureFrame_ % 3) * 20-10)*yureRate_;
	ClearDrawScreen();
	if (yureFrame_ == 0) {
		x = 0;
	}
	//通常描画
	DrawGraph(x, 0, RT_, false);
	

}

void GameScene::FadeDraw()
{
	DrawRotaGraph(320, 240, 1.0f, 0.0f, bgH_, true);
	float rate = static_cast<float>(frame_) /
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, 640, 480, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



GameScene::GameScene(SceneController& controller) :Scene(controller)
{
	RT_ = MakeScreen(640, 480, true);
	bgH_=LoadGraph(L"img/Game/bg_space.png");
	assert(bgH_ >= 0);
	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
	frame_ = fade_interval;
}

GameScene::~GameScene()
{
	DeleteGraph(bgH_);
}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}
