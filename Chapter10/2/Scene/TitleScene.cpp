#include "TitleScene.h"
#include<DxLib.h>
#include<memory>
#include<cassert>
#include"SceneController.h"
#include"GameScene.h"
#include"../Input.h"
namespace {
	constexpr int fade_interval = 60;
}

void TitleScene::FadeInUpdate(Input&)
{
	--frame_;
	if (frame_ <= 0) {
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frame_ = 0;
		return;
	}

}

void TitleScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval) {//�J�ڏ���
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;//�V�[�������ւ����珈����ł��؂�
	}
}

void TitleScene::FadeDraw()
{
	DrawRotaGraph(320, 240, 1.0f, 0.0f, titleH_, true);

	//�t�F�[�h����0�`1�͈̔͂̒l�́u�����v���v�Z���Ă���
	float rate = static_cast<float>(frame_) / 
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawRotaGraph(320, 240, 1.0f, 0.0f, titleH_, true);
	DrawString(10, 10, L"Title Scene", 0xffffff);
}

TitleScene::TitleScene(SceneController& controller):Scene(controller)
{
	titleH_=LoadGraph(L"img/title.png");
	assert(titleH_);
	update_ = &TitleScene::FadeInUpdate;
	draw_ = &TitleScene::FadeDraw;
	frame_ = fade_interval;
}

TitleScene::~TitleScene()
{
	DeleteGraph(titleH_);
}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}
