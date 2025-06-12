#pragma once
#include "Scene.h"
#include<vector>
#include<string>
class GameScene :
    public Scene
{
private:
	int bgH_;
	int frame_ = 0;

	int yureFrame_;
	float yureRate_=0.0f;
	int RT_;//�����_�[�^�[�Q�b�g
	int RTBloom_;//���炷RT
	int RTSmall_;//�k���o�b�t�@�pRT
	int ps_;//�s�N�Z���V�F�[�_

	using UpdateFunc_t = void (GameScene::*)(Input& input);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpdate(Input& input);//�t�F�[�h�C��
	void NormalUpdate(Input& input);//�ʏ�
	void FadeOutUpdate(Input& input);//�t�F�[�h�A�E�g

	void NormalDraw();//�ʏ�`��
	void FadeDraw();//�t�F�[�h�`��

	struct Vector3 {
		float x, y, z;
	};
	struct Location {
		std::string name;//���b�V����
		Vector3 pos;//�ꏊ
		Vector3 angle;//��]
	};
	std::vector<Location> locationData_;
	void LoadLocationData();//�z�u�f�[�^��ǂݍ���

	std::vector<int> models_;

	Vector3 cameraPos_;

public:
	/// <summary>
	/// �V�[��������(�̍ۂɃV�[���R���g���[�������)
	/// </summary>
	/// <param name="controller"></param>
	GameScene(SceneController& controller);
	~GameScene();
	/// <summary>
	/// ���t���[����Ԃ��X�V����
	/// </summary>
	/// <param name="input">���̓N���X</param>
	void Update(Input& input) override;

	/// <summary>
	/// ���t���[���`�悷��
	/// </summary>
	virtual void Draw() override;
};

