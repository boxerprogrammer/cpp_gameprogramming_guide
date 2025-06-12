#include<DxLib.h>
#include "PauseScene.h"
#include"SceneController.h"
#include"../Application.h"
#include"../Input.h"
#include"CommandListScene.h"
#include"KeyConfigScene.h"
#include"SystemSettingScene.h"
#include"TitleScene.h"

using namespace std;

namespace {
	constexpr int appear_interval = 20;//�o���܂ł̃t���[��
	constexpr int input_list_row_height = 40;//���j���[�̂P������̍���
	constexpr int margin_size = 20;//�|�[�Y���j���[�g�̗]��
}

void PauseScene::AppearUpdate(Input& input)
{
	if (++frame_ >= appear_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::ProcessDraw;
		return;
	}
	if (input.IsTriggered("up")) {
		currentIndex_=(currentIndex_+ menuList_.size() -1)%menuList_.size();
	}else if (input.IsTriggered("down")) {
		currentIndex_=(currentIndex_+1)%menuList_.size();
	}
	if (input.IsTriggered("ok")) {
		auto selectedName = menuList_[currentIndex_];
		menuFuncTable_[selectedName](input);
		return;
	}

}

void PauseScene::ProcessDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	int centerY = wsize.height / 2;//��ʒ��SY
	int frameHalfHeight = (wsize.height - margin_size * 2) / 2;//�g�̍����̔���

	//�o���E���Ŏ��̍����ω���(0.0�`1.0)
	float rate = static_cast<float>(frame_) / 
					static_cast<float>(appear_interval);

	frameHalfHeight *= rate;

	//�����ۂ��Z���t�@��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, centerY-frameHalfHeight,
		wsize.width - margin_size, centerY+frameHalfHeight,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//���g
	DrawBoxAA(margin_size, centerY - frameHalfHeight,
		wsize.width - margin_size, centerY + frameHalfHeight,
		0xfffffff, false, 3.0f);
}

void PauseScene::NormalDraw()
{
	
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//�����ۂ��Z���t�@��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//���g
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xfffffff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"Pause Scene", 0x0000ff);
	DrawMenuList();
}

void PauseScene::DrawMenuList()
{
	constexpr int line_start_y = margin_size + 150;
	constexpr int line_start_x = margin_size + 250;
	int lineY = line_start_y;
	
	auto currentStr = menuList_[currentIndex_];
	for (auto& row : menuList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;
		if (row == currentStr) {
			DrawString(lineX - 20, lineY, L"��",0xff0000);
			col = 0xff00ff;
			lineX += 10;
		}
		DrawFormatString(lineX+1, lineY+1, 0x000000, L"%s", row.c_str());
		DrawFormatString(lineX, lineY, col, L"%s", row.c_str());
		lineY += input_list_row_height;
	}
	
}

PauseScene::PauseScene(SceneController& controller):
	Scene(controller),
	frame_(0),
	update_(&PauseScene::AppearUpdate),
	draw_(&PauseScene::ProcessDraw)
{
	menuList_ = { 
					L"�R�}���h�\",
					L"�ݒ胁�j���[",
					L"�L�[�R���t�B�O",
					L"�߂�",
					L"�^�C�g���ɖ߂�"
				};
	menuFuncTable_={
		{L"�R�}���h�\",[this](Input&) {
				controller_.PushScene(make_shared<CommandListScene>(controller_));
			}
		},
		{L"�ݒ胁�j���[",[this](Input&) {
				controller_.PushScene(make_shared<SystemSettingScene>(controller_));
				}},
		{L"�L�[�R���t�B�O",[this](Input& input) {
				controller_.PushScene(make_shared<KeyConfigScene>(controller_,input));
			}},
		{L"�߂�",[this](Input&) {
				update_ = &PauseScene::DisappearUpdate;
				draw_ = &PauseScene::ProcessDraw;
			}
		},
		{L"�^�C�g���ɖ߂�",[this](Input&) {
				controller_.JumpScene(make_shared<TitleScene>(controller_));
			}},
	};
}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}
