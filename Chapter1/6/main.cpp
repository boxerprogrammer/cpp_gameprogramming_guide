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


/// <summary>
/// 上側パイプを描画する
/// </summary>
/// <param name="x">中心X座標</param>
/// <param name="topY">上端(事実上固定)</param>
/// <param name="drawScale">描画スケール(パイプの長さとは別)</param>
/// <param name="length">下端からのパイプの長さ</param>
/// <param name="handle">画像ハンドル</param>
void DrawUpperPipe(int x,int topY,float drawScale,int length,int handle){
	constexpr int edge_graph_height = 20;//パイプの端っこの部分の元画像の高さ
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//これは大きすぎるので切り出し
	gw /= 4;
	gh /= 2;

	int left = x-gw * drawScale/2;//左端
	int right = x + gw * drawScale / 2;//右端
	int top = topY;
	int bottom = topY + length - edge_graph_height*drawScale;//下端(エッジの手前まで)
	//非エッジ(伸び縮みする部分)描画
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//エッジ以外の部分の描画
	//エッジ描画
	DrawRectRotaGraph(x, bottom - edge_graph_height * drawScale/2,
		0, gh-edge_graph_height, gw, edge_graph_height, 
		drawScale,0.0f,handle, true);//エッジ以外の部分の描画

	

}

/// <summary>
/// 下側パイプを描画する
/// </summary>
/// <param name="x">中心X座標</param>
/// <param name="bottomY">下端Y(事実上固定)</param>
/// <param name="drawScale">描画スケール(パイプの長さとは別)</param>
/// <param name="length">下端からのパイプの長さ</param>
/// <param name="handle">画像ハンドル</param>
void DrawLowerPipe(int x,int bottomY,float drawScale,int length,int handle){
	constexpr int edge_graph_height = 20;//パイプの端っこの部分の元画像の高さ
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//これは大きすぎるので切り出し
	gw /= 4;
	gh /= 2;

	int left = x-gw * drawScale/2;//左端
	int right = x + gw * drawScale / 2;//右端
	int top = bottomY - length + edge_graph_height * drawScale;//上端(エッジの手前まで)
	int bottom = bottomY;
	//非エッジ(伸び縮みする部分)描画
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//エッジ以外の部分の描画
	//エッジ描画
	DrawRectRotaGraph(x, bottom-length+edge_graph_height*drawScale/2,
		0, 0, gw, edge_graph_height, 
		drawScale,0.0f,handle, true);//エッジ以外の部分の描画

	DrawBoxAA(left, top, right, bottom, GetColor(255, 0, 0),false,2.0f);//デバッグ用
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
		constexpr int vertical_space_between_pipes = 160;//パイプの上下の間隔
		constexpr int horizontal_space_between_pipes = 80;//パイプの左右の間隔
		for(int i=0;i<10;++i){
			//パイプの長さの計算
			int ySpaceCenter = 240;//パイプ間の通り抜けられるスペースの中心Y座標
			int pipeHeight = ySpaceCenter - vertical_space_between_pipes / 2;

			//パイプ上
			DrawUpperPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,0,
							background_scale, pipeHeight, pipeH);
			//パイプ下
			DrawLowerPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,480,
							background_scale, pipeHeight, pipeH);
		}
		float angle = atan2f(vy, vx);//速度ベクトルを角度に変換
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);
		if(isDead){
			DrawString(200, 240, L"Game over press R!! for continue ", GetColor(255, 0, 0));
		}
		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16ミリ秒経ってなかったら16ミリ秒になるまで待つ
			Sleep((unsigned int)(flip_interval-waitTime));
		}
		ScreenFlip();
	}
	return 0;
}