#include<DxLib.h>
#include<cassert>
#include<cmath>


int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int playerH = LoadGraph(L"../img/bird.png");
	int backH = LoadGraph(L"../img/Background2.png");//背景のロード
	int pipeH = LoadGraph(L"../img/PipeStyle1.png");//パイプのロード

	float y = 240.0f;//自機のY座標
	float vy = 0.0f;//自機のY方向速度
	float vx = 3.0f;//自機のX方向速度
	constexpr float g = 0.3f;//重力加速度
	constexpr float jump_velocity = -6.0f;//ジャンプの瞬間速度(上向き)
	constexpr float background_scale = 480.0f / 256.0f;//背景拡大率


	//ゲームスピードを一定に保つための工夫
	auto lastTime = GetTickCount64();//直前のミリ秒を計っておく
	constexpr auto flip_interval = 16U;//画面更新当たりのミリ秒(1000/60=16.66だが16ミリ秒とする)

	auto lastMouseSate = GetMouseInput();
	int bgPosX = 0;//背景の基準X座標
	int pipePosX = 0;//パイプの基準X座標
	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//クリックされたら空中ジャンプする
		auto currentMouseState = GetMouseInput();
		if(!(lastMouseSate&MOUSE_INPUT_LEFT) && (currentMouseState&&MOUSE_INPUT_LEFT)){
			vy = jump_velocity;
		}
		vy += g;
		y += vy;


		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		DrawRotaGraph(bgPosX+240, 240,background_scale,0.0f, backH, true);//背景1
		DrawRotaGraph(bgPosX+240+480, 240,background_scale,0.0f, backH, true);//背景2
		DrawRotaGraph(bgPosX+240+480+480, 240,background_scale,0.0f, backH, true);//背景3
		for(int i=0;i<10;++i){
			//パイプ上
			DrawRectRotaGraph(pipePosX+(32+8) * background_scale * i + 24 * background_scale, 0, //表示座標
								0,0,32,80,//切り抜き範囲
								background_scale, 0.0f, //拡縮・回転
								pipeH, true);
			//パイプ下
			DrawRectRotaGraph(pipePosX+ (32 + 8) * background_scale * i + 24 * background_scale, 480,//表示座標
								0,0,32,80, //切り抜き範囲
								background_scale, 0.0f, //拡縮・回転
								pipeH, true);
		}
		float angle = atan2f(vy, vx);//速度ベクトルを角度に変換
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);
		bgPosX -= vx;//背景を速度の逆方向にスクロール
		bgPosX = bgPosX % 480;//背景は480ピクセルごとに繰り返す
		pipePosX -= vx;//背景を速度の逆方向にスクロール
		pipePosX = pipePosX % (int)((32+8) * background_scale);//
		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16ミリ秒経ってなかったら16ミリ秒になるまで待つ
			Sleep((unsigned int)waitTime);
		}
		ScreenFlip();
	}
	return 0;
}