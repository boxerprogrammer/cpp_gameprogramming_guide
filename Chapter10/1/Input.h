#pragma once

//#include<map>//�Ή��\�̂���
#include<unordered_map>
#include<string>//�Ή��L�[�̂���
#include<vector>//�f�o�C�X��ǉ����邽��
#include<functional>
#include<DxLib.h>

/// <summary>
/// ���Ӌ@����
/// </summary>
enum class PeripheralType {
	keyboard,//�L�[�{�[�h
	gamepad,//�p�b�h
	mouse,//�}�E�X
	x_analog,//Xbox�R���g���[���̃A�i���O����
	end,//for���ŉ񂷂Ƃ��p�ɒ�`
};

/// <summary>
/// �A�i���O���͎��
/// </summary>
enum class AnalogInputType {
	none,//�����ĂȂ�
	l_up,//���X�e�B�b�N�̏�
	l_down,//���X�e�B�b�N�̉�
	l_left,//���X�e�B�b�N�̍�
	l_right,//���X�e�B�b�N�̉E
	r_up,//���X�e�B�b�N�̏�
	r_down,//���X�e�B�b�N�̉�
	r_left,//���X�e�B�b�N�̍�
	r_right,//���X�e�B�b�N�̉E
	l_trigger,//���g���K�[
	r_trigger,//�E�g���K�[
};
class KeyConfigScene;
class Input
{
	friend KeyConfigScene;//private���������Ă�����
private:
	struct InputState {
		PeripheralType type;//���Ӌ@����
		uint32_t code;//���̓R�[�h(�ėp)
	};
	using InputTable_t = std::unordered_map < std::string, std::vector<InputState>>;
	InputTable_t inputTable_;//�C�x���g�Ǝ��ۂ̓��͂̑Ή��\

	using InputData_t = std::unordered_map < std::string, bool>;
	InputData_t currentInput_;//���̃C�x���g�ɑΉ�����{�^����������Ă��Ԃ�
	InputData_t lastInput_;//���̃C�x���g�ɑΉ�����{�^����������Ă��Ԃ�(���O)

	std::vector<std::string> inputListForDisplay_;

	using AnalogInputTable_t = 
		std::unordered_map<AnalogInputType, std::function<bool(const XINPUT_STATE&)>>;
	AnalogInputTable_t analogInputTable_;

	/// <summary>
	/// ���̓e�[�u���̃Z�[�u
	/// </summary>
	void Save();
	/// <summary>
	/// ���̓e�[�u���̃��[�h
	/// </summary>
	void Load();

public:
	Input();
	void ResetTable();
	/// <summary>
	/// �w�肳�ꂽ�R�[�h�������ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="eventcode">�Ώۂ̃C�x���g�R�[�h</param>
	/// <returns>�������u�ԁFtrue / false ������ĂȂ�or�������p</returns>
	bool IsTriggered(const std::string& eventcode)const;
	bool IsPressed(const std::string& eventcode)const;
	void Update();//���͏�Ԃ��X�V����
};

