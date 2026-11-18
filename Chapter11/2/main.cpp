#include<DxLib.h>
#include"Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::GetInstance();//Applicationの参照を取得

	if (!app.Init()) {//アプリケーションの初期化
		return -1;
	}
	app.Run();//メインループ

	app.Terminate();//終了処理
	return 0;
}