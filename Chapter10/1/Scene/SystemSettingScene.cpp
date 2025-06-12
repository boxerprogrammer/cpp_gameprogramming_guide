#include<DxLib.h>
#include "SystemSettingScene.h"
#include"../Application.h"
#include"SceneController.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;//�o���܂ł̃t���[��
	constexpr int input_list_row_height = 40;//���j���[�̂P������̍���
	constexpr int margin_size = 20;//�|�[�Y���j���[�g�̗]��
}

SystemSettingScene::SystemSettingScene(SceneController& controller):Scene(controller)
{
}

void SystemSettingScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.PopScene();
		return;
	}
}

void SystemSettingScene::Draw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//���ۂ��Z���t�@��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xaaaaff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//��
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0x0000ff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"�V�X�e���ݒ�", 0xff0000);
}
