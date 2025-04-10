#include<DxLib.h>
#include<cassert>
#include<cmath>

#include<algorithm>

/// <summary>
/// 矩形同士の当たり判定
/// </summary>
/// <param name="leftA">Aの左</param>
/// <param name="rightA">Aの右</param>
/// <param name="topA">Aの上</param>
/// <param name="bottomA">Aの下</param>
/// <param name="leftB">Bの左</param>
/// <param name="rightB">Bの右</param>
/// <param name="topB">Bの上</param>
/// <param name="bottomB">Bの下</param>
/// <returns>true:当たっている / false:当たっていない</returns>
bool IsHit(int leftA, int rightA, int topA, int bottomA,
	int leftB, int rightB, int topB, int bottomB) {
	return !(std::min(rightA, rightB) < std::max(leftA, leftB) ||
		std::min(bottomA, bottomB) < std::max(topA, topB));
}

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

	float x = 320.0f;//自機のX座標
	float y = 240.0f;//自機のY座標
	float vx = 3.0f;//自機のX方向速度
	float vy = 0.0f;//自機のY方向速度
	constexpr float player_scale = 3.0f;//自機の拡大率
	constexpr float g = 0.3f;//重力加速度
	constexpr float jump_velocity = -6.0f;//ジャンプの瞬間速度(上向き)
	constexpr float background_scale = 480.0f / 256.0f;//背景拡大率


	//ゲームスピードを一定に保つための工夫
	auto lastTime = GetTickCount64();//直前のミリ秒を計っておく
	constexpr auto flip_interval = 16U;//画面更新当たりのミリ秒(1000/60=16.66だが16ミリ秒とする)

	auto lastMouseSate = GetMouseInput();
	int bgPosX = 0;//背景の基準X座標
	int pipePosX = 0;//パイプの基準X座標
	
	bool isDead = false;

	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//クリックされたら空中ジャンプする
		auto currentMouseState = GetMouseInput();

		if (isDead) {
			if (CheckHitKey(KEY_INPUT_R)) {
				isDead = false;
				x = 320.0f;//自機のX座標
				y = 240.0f;//自機のY座標
				vy = 0.0f;//自機のY方向速度
				bgPosX = 0;
				pipePosX = 0;
			}
		}
		if (!isDead) {
			if (!(lastMouseSate & MOUSE_INPUT_LEFT) && (currentMouseState && MOUSE_INPUT_LEFT)) {
				vy = jump_velocity;
			}
			vy += g;
			y += vy;
			bgPosX -= vx;//背景を速度の逆方向にスクロール
			bgPosX = bgPosX % 480;//背景は480ピクセルごとに繰り返す
			pipePosX -= vx;//背景を速度の逆方向にスクロール
			pipePosX = pipePosX % (int)((32 + 8) * background_scale);//
		}
		//プレイヤー上下左右
		int playerL = static_cast<int>(x - 8.0f * player_scale);//自機左
		int playerR = static_cast<int>(x + 8.0f * player_scale);//自機右
		int playerT = static_cast<int>(y- 8.0f * player_scale);//自機上
		int playerB = static_cast<int>(y + 8.0f * player_scale);//自機下
		//当たり判定
		for (int i = 0; i < 10; ++i) {
			//パイプ上と判定
			int upperPipeCenterX = pipePosX + (32 + 8) * background_scale * i + 24 * background_scale;
			int upperPipeCenterY = 0;
			if (IsHit(
				upperPipeCenterX - 16 * background_scale,//パイプ左
				upperPipeCenterX + 16 * background_scale,//パイプ右
				upperPipeCenterY - 40 * background_scale,//パイプ上
				upperPipeCenterY + 40 * background_scale,//パイプ下
				playerL,//自機左
				playerR,//自機右
				playerT,//自機上
				playerB)) {//自機下
				isDead = true;
				break;
			}

			//パイプ下と判定
			int lowerPipeCenterX = pipePosX + (32 + 8) * background_scale * i + 24 * background_scale;
			int lowerPipeCenterY = 480;
			if (IsHit(
				lowerPipeCenterX - 16 * background_scale,//パイプ左
				lowerPipeCenterX + 16 * background_scale,//パイプ右
				lowerPipeCenterY - 40 * background_scale,//パイプ上
				lowerPipeCenterY + 40 * background_scale,//パイプ下
				playerL,//自機左
				playerR,//自機右
				playerT,//自機上
				playerB)) {//自機下
				isDead = true;
				break;
			}
		}



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

		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16ミリ秒経ってなかったら16ミリ秒になるまで待つ
			Sleep((unsigned int)waitTime);
		}
		ScreenFlip();
	}
	return 0;
}