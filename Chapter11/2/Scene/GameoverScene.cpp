#include<DxLib.h>
#include<cassert>

#include"../Input.h"
#include"../Application.h"

#include "GameoverScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"GameScene.h"

constexpr int fade_interval = 40;
void GameoverScene::FadeInUpdate(Input&)
{
	if (--frame_ <= 0) {
		update_ = &GameoverScene::NormalUpdate;
		draw_ = &GameoverScene::NormalDraw;
		frame_=0;
	}
}
void GameoverScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &GameoverScene::FadeOutUpdate;
		draw_ = &GameoverScene::FadeDraw;
		frame_ = 0;
		return;
	}
	if (input.IsTriggered("back")) {
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;
	}
}
void GameoverScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval) {
		//Changeした時点で、GameoverSceneは削除されています
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}
void GameoverScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	NormalDraw();
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void GameoverScene::NormalDraw()
{
	DrawGraph(0, 0, backH_, true);
	DrawGraph(0, 200, handle_, true);
	DrawString(10, 10, L"Gameover Scene...", 0xffffff);
}
GameoverScene::GameoverScene(SceneController& ctrl):Scene(ctrl)
{
	handle_ = LoadGraph(L"img/gameover.png");
	assert(handle_ >= 0);
	backH_ = LoadGraph(L"img/gameover_back.png");
	assert(backH_ >= 0);
	update_ = &GameoverScene::FadeInUpdate;
	draw_ = &GameoverScene::FadeDraw;
	frame_ = fade_interval;
}

GameoverScene::~GameoverScene()
{
	DeleteGraph(backH_);
}

void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameoverScene::Draw()
{
	(this->*draw_)();
}
