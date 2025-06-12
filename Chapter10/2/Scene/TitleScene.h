#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
private:
	int titleH_;
	int frame_ = 0;
	//�����o�֐��|�C���^�^�̕ʖ����`
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();
	
	UpdateFunc_t update_;//Update�p�̃����o�֐��|�C���^
	DrawFunc_t draw_;//Draw�p�̃����o�֐��|�C���^

	//�؂�ւ���ׂ��֐��������Ă���
	void FadeInUpdate(Input&);//�t�F�[�h�C��
	void NormalUpdate(Input&);//�ʏ�(���͎�t)
	void FadeOutUpdate(Input&);//�t�F�[�h�A�E�g

	void FadeDraw();//�t�F�[�h���̕`��
	void NormalDraw();//�ʏ펞�̕`��

public:
	TitleScene(SceneController& controller);
	~TitleScene();
	/// <summary>
	/// ���t���[����Ԃ��X�V����
	/// </summary>
	/// <param name="input">���̓N���X</param>
	virtual void Update(Input& input)override;

	/// <summary>
	/// ���t���[���`�悷��
	/// </summary>
	virtual void Draw()override;
};

