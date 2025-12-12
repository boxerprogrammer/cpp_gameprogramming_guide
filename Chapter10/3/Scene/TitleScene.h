#pragma once
#include "Scene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene//Sceneから継承したTitleScene
{
private:
	int titleH_;//タイトル画像ハンドル
	int titleLogoH_;//タイトルロゴハンドル
	int frame_;//経過フレーム

	//メンバ関数ポインタを使用して内部状態を書き換える
	//update関数とdraw関数を切り替える
	void FadeInUpdate(Input&);//フェードインUpdate
	void NormalUpdate(Input&);//通常Update
	void FadeOutUpdate(Input&);//フェードアウトUpdate

	void (TitleScene::* func_)(Input&);

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	UpdateFunc_t update_;//Update関数を代入できるメンバ関数ポインタ


	void NormalDraw();//通常Draw
	void FadeDraw();//フェードDraw
	using DrawFunc_t = void(TitleScene::*)();
	DrawFunc_t draw_;//Draw関数を代入できるメンバ関数ポインタ
public:
	TitleScene(SceneController&);
	void Update(Input&)override;
	void Draw()override;
};

