#include<DxLib.h>
#include "KeyConfigScene.h"
#include"../Application.h"
#include"SceneController.h"
#include"../Input.h"
#include"../StringUtiligy.h"

#include<algorithm>

namespace {
	constexpr int appear_interval = 20;//�o���܂ł̃t���[��
	constexpr int input_list_row_height = 40;//�C�x���g�̂P������̍���
	constexpr int input_list_col_width = 150;//���蓖�ăR�[�h��ɗv���镝
	constexpr int margin_size = 20;//�|�[�Y���j���[�g�̗]��
	constexpr int input_list_top = 100;//���͏��̏�[
	constexpr int input_list_left = 70;//���͏��̍��[
	constexpr int analog_lever_threshold = 10000;//�A�i���O���o�[��|����臒l
	constexpr int analog_trigger_threshold = 128;//�A�i���O�g���K�[��臒l
}

void KeyConfigScene::DrawInputList()
{
	int rowY = input_list_top;//�ŏ���Y���W
	constexpr uint32_t normal_text_color = 0x000000;//�����̒ʏ�J���[
	constexpr uint32_t selected_text_color = 0x0000aa;//���ݑI�𒆂̕����J���[
	constexpr uint32_t editting_text_color = 0xcc0000;//���ݕҏW���̕����J���[
	constexpr uint32_t indicator_offset = 20;//���̃I�t�Z�b�g
	
	for (const auto& keyStr : input_.inputListForDisplay_) {
		auto eventColor = normal_text_color;
		auto eventName = StringUtiligy::GetWStringFromString(keyStr);//�C�x���g��	
		int colX = input_list_left;
		//���݂̃C���f�b�N�X���C�x���g�������Ă���΁A���̍s�̐F��I�𒆂ɕύX����
		if (currentIndex_ < input_.inputListForDisplay_.size()) {
			if (input_.inputListForDisplay_[currentIndex_] == keyStr) {
				DrawString(input_list_left - indicator_offset + 1, rowY + 1, L"��", 0x000000);
				DrawString(input_list_left - indicator_offset, rowY, L"��", 0xaa0000, 0xfffffff);
				colX += 10;
				//�������A���݂��ҏW���[�h�ɓ����Ă���΁A�܂��ʂ̐F�ɂ���
				if (update_ == &KeyConfigScene::EdittingUpdate) {
					eventColor = editting_text_color;
					colX += 10;
				}
				else {
					eventColor = selected_text_color;
				}
			}
		}
		DrawFormatString(colX, rowY, eventColor, L"%s", eventName.c_str());
		colX += 70;
		
		for (auto& record : tempInputTable_[keyStr]) {
			std::wstring periName = GetPeriphString(record.type);//���Ӌ@�햼
			if (record.type == PeripheralType::keyboard) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), keyboardNameTable_[record.code].c_str());
			}
			else if (record.type == PeripheralType::gamepad) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), padNameTable_[record.code].c_str());
			}
			else if (record.type == PeripheralType::mouse) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%x", periName.c_str(), record.code);
			}
			else if (record.type == PeripheralType::x_analog) {
				DrawFormatString(colX, rowY, eventColor,
					L"%s:%s", periName.c_str(), analogNameTable_[ record.code].c_str());
			}
			colX += input_list_col_width;
		}
		rowY += input_list_row_height;
	}

	rowY += 30;//�V�X�e�����j���[�̓L�[�R���t�B�O�������炿����Ɨ���

	//�L�[�R���t�B�O�V�X�e�����j���[��\��
	for (const auto& menuName : systemMenuStringList_) {
		uint32_t texCol = normal_text_color;
		int colX = input_list_left + 100;
		int idx = currentIndex_ - input_.inputListForDisplay_.size();
		if (idx >= 0) {
			if (systemMenuStringList_[idx]==menuName) {
				DrawString(input_list_left - indicator_offset + 100 + 1, rowY + 1, L"��", 0x000000);
				DrawString(input_list_left - indicator_offset + 100, rowY, L"��", 0xaa0000, 0xfffffff);
				colX += 10;
				texCol = selected_text_color;
			}
		}
		DrawString(colX, rowY, menuName.c_str(), texCol);
		rowY += input_list_row_height;
	}
	
}

std::wstring 
KeyConfigScene::GetPeriphString(const PeripheralType& type)
{
	switch (type) {
	case PeripheralType::keyboard:
		return L"keybd";
	case PeripheralType::gamepad:
		return L"pad";
	case PeripheralType::mouse:
		return L"mouse";
	case PeripheralType::x_analog:
		return L"x_analog";
	default:
		return L"";
	}
}

void KeyConfigScene::NormalUpdate(Input& input)
{
	const auto keyEventSize = input.inputListForDisplay_.size();
	const auto inputRowSize = keyEventSize + systemMenuStringList_.size();//�V�X�e�����j���[�𑫂�
	if (input.IsTriggered("ok")) {
		if (currentIndex_ < keyEventSize) {
			GetHitKeyStateAll(lastKeyState_.data());
			lastPadState_= GetJoypadInputState(DX_INPUT_PAD1);
			update_ = &KeyConfigScene::EdittingUpdate;//�ҏW���[�h
			return;
		}
		else {
			const auto& menuStr = systemMenuStringList_[currentIndex_ - keyEventSize];
			systemMenuTable_[menuStr]();
		}
	}
	
	if (input.IsTriggered("up")) {
		currentIndex_ = (currentIndex_ + inputRowSize - 1) % inputRowSize;
	}
	if (input.IsTriggered("down")) {
		currentIndex_ = (currentIndex_ + 1) % inputRowSize;
	}


}

void KeyConfigScene::EdittingUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &KeyConfigScene::NormalUpdate;
		currentKeyState_ = {};
		lastKeyState_ = {};
		currentPadState_ = 0;
		lastPadState_ = 0;
		return;
	}
	GetHitKeyStateAll(currentKeyState_.data());
	int pressedKeyCode = -1;
	for (int i = 0; i < 256; ++i) {
		if (currentKeyState_[i]&&!lastKeyState_[i]) {
			pressedKeyCode = i;
			break;
		}
	}
	currentPadState_ = GetJoypadInputState(DX_INPUT_PAD1);
	int pressedPadCode = 0;
	for (int i = 0; i < 32; ++i) {
		if ((currentPadState_ & (0b1 << i)) != 0 && (lastPadState_ & (0b1 << i)) == 0) {
			pressedPadCode = (0b1 << i);
		}
	}
	//�A�i���O
	XINPUT_STATE xinputState = {};
	AnalogInputType pressedAnalogCode = AnalogInputType::none;
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);
	if (abs(xinputState.ThumbLX) > analog_lever_threshold) {//�����o�[�`�F�b�N
		pressedAnalogCode = xinputState.ThumbLX > 0 ? AnalogInputType::l_right : AnalogInputType::l_left;
	}else if (abs(xinputState.ThumbLY) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbLY > 0 ? AnalogInputType::l_up : AnalogInputType::l_down;
	}else if (abs(xinputState.ThumbRX) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbRX > 0 ? AnalogInputType::r_right : AnalogInputType::r_left;
	}else if (abs(xinputState.ThumbRY) > analog_lever_threshold) {
		pressedAnalogCode = xinputState.ThumbRY > 0 ? AnalogInputType::r_up : AnalogInputType::r_down;
	}else if (abs(xinputState.LeftTrigger) > analog_trigger_threshold) {
		pressedAnalogCode = AnalogInputType::l_trigger;
	}else if (abs(xinputState.RightTrigger) > analog_trigger_threshold) {
		pressedAnalogCode = AnalogInputType::r_trigger;
	}

	
	auto& record = tempInputTable_[input.inputListForDisplay_[currentIndex_]];

	
	//for (auto& state : record) {
	for(int i=0;i<(int)PeripheralType::end; ++i) {
		auto it = std::find_if(record.begin(), record.end(), [i](const Input::InputState& state) {
			return PeripheralType(i) == state.type;
			});
		if (it != record.end()) {
			auto& state = *it;
			if (state.type == PeripheralType::keyboard && pressedKeyCode >= 0) {
				state.code = pressedKeyCode;
			}
			else if (state.type == PeripheralType::gamepad && pressedPadCode > 0) {
				state.code = pressedPadCode;
			}
			else if (state.type == PeripheralType::x_analog && pressedAnalogCode != AnalogInputType::none) {
				state.code = (int)pressedAnalogCode;
			}
		}
		else {
			Input::InputState state = {};
			
			if ((PeripheralType)i == PeripheralType::keyboard && pressedKeyCode >= 0) {
				state.code = pressedKeyCode;
			}
			else if ((PeripheralType)i  == PeripheralType::gamepad && pressedPadCode > 0) {
				state.code = pressedPadCode;
			}
			else if ((PeripheralType)i == PeripheralType::x_analog && pressedAnalogCode != AnalogInputType::none) {
				state.code = (int)pressedAnalogCode;
			}
			if (state.code > 0) {
				state.type = (PeripheralType)i;
				record.push_back(state);
			}
			
		}
	}
	lastKeyState_ = currentKeyState_;
	lastPadState_ = currentPadState_;
}

KeyConfigScene::KeyConfigScene(SceneController& controller, Input& input) :
	Scene(controller),
	input_(input),
	update_(&KeyConfigScene::NormalUpdate)
{
	ReloadTable();

	keyboardNameTable_[KEY_INPUT_A] = L"�`�L�[";
	keyboardNameTable_[KEY_INPUT_B] = L"�a�L�[";
	keyboardNameTable_[KEY_INPUT_C] = L"�b�L�[";
	keyboardNameTable_[KEY_INPUT_D] = L"�c�L�[";
	keyboardNameTable_[KEY_INPUT_E] = L"�d�L�[";
	keyboardNameTable_[KEY_INPUT_F] = L"�e�L�[";
	keyboardNameTable_[KEY_INPUT_G] = L"�f�L�[";
	keyboardNameTable_[KEY_INPUT_H] = L"�g�L�[";
	keyboardNameTable_[KEY_INPUT_I] = L"�h�L�[";
	keyboardNameTable_[KEY_INPUT_J] = L"�i�L�[";
	keyboardNameTable_[KEY_INPUT_K] = L"�j�L�[";
	keyboardNameTable_[KEY_INPUT_L] = L"�k�L�[";
	keyboardNameTable_[KEY_INPUT_M] = L"�l�L�[";
	keyboardNameTable_[KEY_INPUT_N] = L"�m�L�[";
	keyboardNameTable_[KEY_INPUT_O] = L"�n�L�[";
	keyboardNameTable_[KEY_INPUT_P] = L"�o�L�[";
	keyboardNameTable_[KEY_INPUT_Q] = L"�p�L�[";
	keyboardNameTable_[KEY_INPUT_R] = L"�q�L�[";
	keyboardNameTable_[KEY_INPUT_S] = L"�r�L�[";
	keyboardNameTable_[KEY_INPUT_T] = L"�s�L�[";
	keyboardNameTable_[KEY_INPUT_U] = L"�t�L�[";
	keyboardNameTable_[KEY_INPUT_V] = L"�u�L�[";
	keyboardNameTable_[KEY_INPUT_W] = L"�v�L�[";
	keyboardNameTable_[KEY_INPUT_X] = L"�w�L�[";
	keyboardNameTable_[KEY_INPUT_Y] = L"�x�L�[";
	keyboardNameTable_[KEY_INPUT_Z] = L"�y�L�[";
	keyboardNameTable_[KEY_INPUT_BACK] = L"BS�L�[";
	keyboardNameTable_[KEY_INPUT_TAB] = L"Tab�L�[";
	keyboardNameTable_[KEY_INPUT_RETURN] = L"Enter�L�[";
	keyboardNameTable_[KEY_INPUT_ESCAPE] = L"Esc�L�[";
	keyboardNameTable_[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";
	keyboardNameTable_[KEY_INPUT_F1] = L"�e�P�L�[";
	keyboardNameTable_[KEY_INPUT_F2] = L"�e�Q�L�[";
	keyboardNameTable_[KEY_INPUT_F3] = L"�e�R�L�[";
	keyboardNameTable_[KEY_INPUT_F4] = L"�e�S�L�[";
	keyboardNameTable_[KEY_INPUT_F5] = L"�e�T�L�[";

	padNameTable_[PAD_INPUT_A] = L"�`�{�^��";
	padNameTable_[PAD_INPUT_B] = L"�a�{�^��";
	padNameTable_[PAD_INPUT_C] = L"�w�{�^��";
	padNameTable_[PAD_INPUT_X] = L"�x�{�^��";
	padNameTable_[PAD_INPUT_Y] = L"�k�{�^��";
	padNameTable_[PAD_INPUT_Z] = L"�q�{�^��";
	padNameTable_[PAD_INPUT_L] = L"�Z���N�g�{�^��";
	padNameTable_[PAD_INPUT_R] = L"�X�^�[�g�{�^��";
	padNameTable_[PAD_INPUT_START] = L"�k�X�e�B�b�N";
	padNameTable_[PAD_INPUT_M] = L"�q�X�e�B�b�N";

	analogNameTable_[(int)AnalogInputType::l_up] = L"�����o�[��";
	analogNameTable_[(int)AnalogInputType::l_down] = L"�����o�[��";
	analogNameTable_[(int)AnalogInputType::l_left] = L"�����o�[��";
	analogNameTable_[(int)AnalogInputType::l_right] = L"�����o�[�E";
	analogNameTable_[(int)AnalogInputType::r_up] = L"�E���o�[��";
	analogNameTable_[(int)AnalogInputType::r_down] = L"�E���o�[��";
	analogNameTable_[(int)AnalogInputType::r_left] = L"�E���o�[��";
	analogNameTable_[(int)AnalogInputType::r_right] = L"�E���o�[�E";
	analogNameTable_[(int)AnalogInputType::l_trigger] = L"���g���K�[";
	analogNameTable_[(int)AnalogInputType::r_trigger] = L"�E�g���K�[";
	
	systemMenuTable_[L"SAVE&EXIT"] = [this]() {
		CommitInputTable();
		controller_.PopScene();
	};
	systemMenuTable_[L"CANCEL&EXIT"] = [this]() {
		controller_.PopScene();
	};
	systemMenuTable_[L"RESET"] = [this]() {
		input_.ResetTable();
		ReloadTable();
	};

	systemMenuStringList_ = { L"RESET",L"SAVE&EXIT" ,L"CANCEL&EXIT" };

}

void KeyConfigScene::ReloadTable()
{
	tempInputTable_ = input_.inputTable_;
}

void KeyConfigScene::Update(Input& input)
{
	(this->*update_)(input);

}

void KeyConfigScene::Draw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//�΂��ۂ��Z���t�@��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xaaffaa, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//��
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0x00ff00, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"�L�[�R���t�B�O", 0x000000);
	
	DrawInputList();
}

void KeyConfigScene::CommitInputTable()
{
	input_.inputTable_ = tempInputTable_;
	input_.Save();
}
