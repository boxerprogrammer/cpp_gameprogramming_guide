#pragma once
#include "Scene.h"
#include<vector>
#include<map>
#include<functional>
#include<string>
/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
private:
	std::vector<std::wstring> menuItems_;
	std::map<std::wstring, std::function<void(Input&)>> menuFunctions_;

	int currentIndex_=0;//今選択中のインデックス

	int yesNoIndex_ = 1;//通常は「1:いいえ」
	std::vector<std::wstring> yesNoItems_;
	std::function<void()> deferredExec_;//遅延実行する関数


	int frame_;
	//メンバ関数ポインタ変数を作る
	using UpdateFunc_t = void (PauseScene::*)(Input&);
	using DrawFunc_t = void (PauseScene::*)();
	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//出現時更新
	void AppearUpdate(Input&);
	//通常
	void NormalUpdate(Input& input);

	//確認画面
	void YesNoUpdate(Input& input);

	//退場時更新
	void DisappearUpdate(Input&);

	//通常描画
	void NormalDraw();

	//枠だけ表示
	void FrameOnlyDraw();
	//確認画面描画
	void YesNoDraw();

public:
	PauseScene(SceneController& ctrl);
	void Update(Input& input)override;
	void Draw()override;
};

