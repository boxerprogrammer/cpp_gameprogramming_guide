#include<DxLib.h>
#include"Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::GetInstance();
	if (!app.Init()) {//初期化処理
		return -1;
	}
	app.Run();//ゲームスタート(ゲームループに入る)
	app.Terminate();//後処理
	return 0;
}