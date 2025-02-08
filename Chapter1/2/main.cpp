#include<DxLib.h>
#include<cassert>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int playerH = LoadGraph(L"../img/Abeja.png");

	float y = 240.0f;//自機のY座標
	float vy = 0.0f;//自機のY方向速度
	constexpr float g = 0.3f;//重力加速度
	constexpr float jump_velocity = -6.0f;//ジャンプの瞬間速度(上向き)

	//ゲームスピードを一定に保つための工夫
	auto lastTime = GetTickCount64();//直前のミリ秒を計っておく
	constexpr auto flip_interval = 16U;//画面更新当たりのミリ秒(1000/60=16.66だが16ミリ秒とする)

	auto lastMouseSate = GetMouseInput();

	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//クリックされたら空中ジャンプする(ぴょん)
		auto currentMouseState = GetMouseInput();
		if(!(lastMouseSate&MOUSE_INPUT_LEFT) && (currentMouseState&&MOUSE_INPUT_LEFT)){
			vy = jump_velocity;
		}

		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		DrawRectRotaGraphF(320.0f, y,0,0,240,240,0.3f,0.0f, playerH, true);
		vy += g;
		y += vy;
		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16ミリ秒経ってなかったら16ミリ秒になるまで待つ
			Sleep((unsigned int)waitTime);
		}
		ScreenFlip();
	}
	return 0;
}