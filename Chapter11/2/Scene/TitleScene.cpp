#include<DxLib.h>
#include<cassert>

#include"../Input.h"
#include"../Application.h"
#include"../FileLoader.h"

#include"SceneController.h"
#include "TitleScene.h"
#include"GameScene.h"


constexpr int fade_interval = 30;

void TitleScene::FadeInUpdate(Input&)
{
	//最初30でだんだん減っていって0以下になれば次の状態(通常)に遷移する
	if (--frame_ <=0) {
		//通常状態に遷移します
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frame_ = 0;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval) {
		//ChangeSceneが完了した時点で、自分は死んでいます。
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		//この後に処理を行うと必ず失敗します。なので、速やかにreturnします
		return;
	}
}

void TitleScene::FadeDraw()
{
	NormalDraw();//通常描画

	//画面全体を覆う黒いセロファンをアルファ値を変更しながら表示する
	//経過時間を0.0～1.0に変換している
	//フェードインのときは1.0→0.0に変化
	//フェードアウトのときは0.0→1.0に変化
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	//黒セロファンの濃度を変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//黒セロファンを描画
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void TitleScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawExtendGraph(0, 0,wsize.w,wsize.h, backH_, true);
	DrawGraph(50, 50, handle_, true);
	DrawString(10, 10, L"Title Scene...", 0xffffff);
}

TitleScene::TitleScene(SceneController& ctrl):Scene(ctrl)
{
	auto loader = Application::GetInstance().GetLoader();
	indicatorH_ = loader->LoadGraph(L"img/Common/asteroid-small.png", AssetType::continuous);
	backH_ = loader->LoadGraph(L"img/title_bg.jpg",AssetType::temporary);
	

	//最初の状態を代入してあげる
	update_ = &TitleScene::FadeInUpdate;//フェードイン状態
	draw_ = &TitleScene::FadeDraw;//フェード描画
	frame_ = fade_interval;
}

TitleScene::~TitleScene()
{
	auto loader = Application::GetInstance().GetLoader();
	loader->DeleteTemporalAssets();
}

void TitleScene::Update(Input& input)
{
	//メンバ関数に格納されたUpdate関数を呼び出す
	// (中身がなにかはわからない、ある意味ポリモーフィズム)
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	//メンバ変数に格納されたDraw関数を呼び出す
	(this->*draw_)();
}
