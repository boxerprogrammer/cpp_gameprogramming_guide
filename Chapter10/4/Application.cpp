#include "Application.h"
#include<DxLib.h>
#include<cassert>	
#include"Input.h"
#include"Scene/SceneController.h"
#include"Scene/TitleScene.h"

constexpr int default_window_width = 640;//デフォルトウィンドウ幅
constexpr int default_window_height = 480;//デフォルトウィンドウ高
constexpr int default_color_bit = 32;//デフォルトカラービット

Application::Application() :
	windowSize_{default_window_width,default_window_height} {

}

Application::~Application()
{
}

Application& Application::GetInstance()
{
	static Application instance;//staticで普通に宣言
	return instance;//そしてそれを返す
}

bool Application::Init()
{
	SetWindowSize(windowSize_.w, windowSize_.h);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return false;
	}
	return true;
}

void Application::Run()
{
	SetDrawScreen(DX_SCREEN_BACK);
	Input input;//入力のためのオブジェクト
	SceneController controller;//シーン遷移のためのコントロールオブジェクト
	//最初のシーンをセットする
	//何かしらシーンがないと、UpdateもDrawもできないため
	controller.ChangeScene(std::make_shared<TitleScene>(controller));
	//ゲームループ
	//このループを抜ける＝ゲームが終了する
	while (ProcessMessage() != -1 && !requestedExit_) {
		ClearDrawScreen();
		input.Update();//入力情報の更新

		//シーンの更新
		controller.Update(input);
		//シーンの描画
		controller.Draw();

		ScreenFlip();
	}
}

void Application::Terminate()
{
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}

void Application::RequestExit()
{
	requestedExit_ = true;
}
