#pragma once
#include "Scene.h"
class OverScene :
    public Scene
{
	private:
		int imgH_ = 0;
		int frame_ = 0;
		using UpdateFunc_t = void(OverScene::*)(Input& input);
		using DrawFunc_t = void(OverScene::*)();

		UpdateFunc_t update_;
		DrawFunc_t draw_;

		void FadeInUpadte(Input&);
		void NormalUpadte(Input&);
		void FadeOutUpadte(Input&);

		void NormalDraw();
		void FadeDraw();

	public:
		OverScene(SceneController& controller);
		~OverScene();
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

