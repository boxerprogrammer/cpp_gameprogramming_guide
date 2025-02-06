#include<DxLib.h>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int playerH = LoadGraph(L"img/Abeja.png");

	float vy = 0.0f;
	float g = 0.1f;

	while(ProcessMessage()!=-1){
		ClearDrawScreen();
		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		DrawRectRotaGraph(320, 240,0,0,240,240,0.5f,0.0f, playerH, true);
		ScreenFlip();
	}
	return 0;
}