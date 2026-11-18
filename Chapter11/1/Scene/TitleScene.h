#pragma once
#include "Scene.h"
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	int handle_;//タイトル画面
	int backH_;//背景画像

	int indicatorH_;//指ししめ画像

	int frame_ = 0;//フェードの時等に使用する毎フレームカウントされる変数

	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t update_;//Update関数用メンバ関数ポインタ
	DrawFunc_t draw_;//Draw関数用メンバ関数ポインタ

	//Updateに使う関数
	void FadeInUpdate(Input&);//フェードイン中
	void NormalUpdate(Input& input);//通常(入力受付)
	void FadeOutUpdate(Input&);//フェードアウト中

	//Drawに使う関数
	void FadeDraw();//フェード中
	void NormalDraw();//通常



public:
	TitleScene(SceneController& ctrl);
	~TitleScene();

	/// <summary>
	/// タイトルシーンの更新
	/// </summary>
	/// <param name="input">入力オブジェクト</param>
	void Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
};

