#pragma once
#include "Scene.h"//基底クラスをインクルード

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameoverScene : public Scene//基底クラスをpublic継承します
{
private:
	int handle_;
	int backH_;//背景画像
	int frame_ = 0;
	using UpdateFunc_t = void(GameoverScene::*)(Input&);
	using DrawFunc_t = void(GameoverScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	void FadeDraw();
	void NormalDraw();

public:
	GameoverScene(SceneController& ctrl);
	~GameoverScene();
	void Update(Input& input)override;//親クラスの純粋仮想関数を派生クラスで実装
	void Draw()override;//
};

