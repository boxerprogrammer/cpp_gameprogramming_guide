#include<DxLib.h>
#include<cassert>
#include<cmath>

#include<algorithm>

constexpr int edge_graph_height = 20;//パイプの端っこの部分の元画像の高さ
/// <summary>
/// 矩形を表す構造体
/// </summary>
struct Rect {
	int left;//左
	int top;//上
	int right;//右
	int bottom;//下
};

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
/// 矩形同士の当たり判定(矩形構造体版)
/// </summary>
/// <param name="rcA">矩形A</param>
/// <param name="rcB">矩形B</param>
/// <param name="rcBOffsetX">矩形Bのオフセット</param>
/// <returns></returns>
bool IsHit(const Rect& rcA, const Rect& rcB,int rcBOffsetX=0) {
	return IsHit(rcA.left, rcA.right, rcA.top, rcA.bottom,
		rcB.left+rcBOffsetX, rcB.right+rcBOffsetX, rcB.top, rcB.bottom);
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
/// 上側パイプを描画する(矩形指定版)
/// </summary>
/// <param name="offsetX">中心X座標</param>
/// <param name="drawScale">描画スケール</param>
/// <param name="rc">矩形</param>
/// <param name="handle">画像ハンドル</param>
void DrawUpperPipe(int offsetX,float drawScale,const Rect& rc,int handle){
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//これは大きすぎる(8本文ある)ので切り出し
	gw /= 4;
	gh /= 2;

	int left = rc.left + offsetX;//左端
	int right = rc.right+offsetX;//右端
	int top = rc.top;
	int bottom = rc.bottom - edge_graph_height*drawScale;//下端(エッジの手前まで)
	//非エッジ(伸び縮みする部分)描画
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//エッジ以外の部分の描画
	//エッジ描画
	DrawRectExtendGraph(left , bottom,right,rc.bottom,
		0, gh-edge_graph_height, gw, edge_graph_height, handle, true);
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

/// <summary>
/// 下側パイプを描画する(矩形指定版)
/// </summary>
/// <param name="offsetX">中心X座標</param>
/// <param name="drawScale">描画スケール</param>
/// <param name="rc">矩形</param>
/// <param name="handle">画像ハンドル</param>
void DrawLowerPipe(int offsetX,float drawScale,const Rect& rc,int handle){
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//これは大きすぎるので切り出し
	gw /= 4;
	gh /= 2;

	int left = rc.left + offsetX;//左端
	int right = rc.right+offsetX;//右端
	int top = rc.top + edge_graph_height*drawScale;//下端(エッジの手前まで);
	int bottom = rc.bottom;//下端
	//非エッジ(伸び縮みする部分)描画
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//エッジ以外の部分の描画
	//エッジ描画
	DrawRectExtendGraph(left , rc.top,right,top,
		0, 0, gw, edge_graph_height, handle, true);
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

	Rect upperPipeRects[8]={};//上側パイプの矩形
	Rect lowerPipeRects[8]={};//下側パイプの矩形
	int pipeStartIndex = 0;//リングバッファ用のパイプのインデックス

	int ySpaceCenter = 240;//パイプ間の通り抜けられるスペースの中心Y座標
	constexpr int pipe_start_x = 320;//パイプの開始X座標
	constexpr int vertical_space_between_pipes = 200;//パイプの上下の間隔
	constexpr int horizontal_space_between_pipes = 200;//パイプの左右の間隔
	//初回のパイプ8本の矩形を生成
	for (int i = 0; i < 8; ++i) {

		//上側パイプ
		upperPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
		upperPipeRects[i].top = 0;
		upperPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
		upperPipeRects[i].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
		//下側パイプ
		lowerPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
		lowerPipeRects[i].top = ySpaceCenter + vertical_space_between_pipes / 2;
		lowerPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
		lowerPipeRects[i].bottom = 480;

		ySpaceCenter += (GetRand(40) - 20)*2;//パイプの上下の間隔
	}
	int highScore = 0;//ハイスコア
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
				pipeStartIndex = 0;
				ySpaceCenter = 240;
				//初回のパイプ8本の矩形を生成
				for (int i = 0; i < 8; ++i) {

					//上側パイプ
					upperPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
					upperPipeRects[i].top = 0;
					upperPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
					upperPipeRects[i].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
					//下側パイプ
					lowerPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
					lowerPipeRects[i].top = ySpaceCenter + vertical_space_between_pipes / 2;
					lowerPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
					lowerPipeRects[i].bottom = 480;

					int diff = (GetRand(40) - 20)*2;
					ySpaceCenter += diff;//パイプの上下の間隔
					if (ySpaceCenter - vertical_space_between_pipes / 2 < 0 || ySpaceCenter+vertical_space_between_pipes/2>480) {
						ySpaceCenter -=diff*2;
					}
				}
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
			//pipePosX = pipePosX % (int)((32 + 8) * background_scale);//
		}
		//プレイヤー上下左右
		Rect playerRect = {};
		playerRect.left = static_cast<int>(x - 6.0f * player_scale);//自機左
		playerRect.right = static_cast<int>(x + 6.0f * player_scale);//自機右
		playerRect.top = static_cast<int>(y- 6.0f * player_scale);//自機上
		playerRect.bottom = static_cast<int>(y + 6.0f * player_scale);//自機下

		//当たり判定
		for (int i = 0; i < 8; ++i) {
			//パイプ上と判定
			if (IsHit(playerRect,upperPipeRects[i],pipePosX)) {//自機下
				isDead = true;
				break;
			}
			//パイプ下と判定
			if (IsHit(playerRect,lowerPipeRects[i],pipePosX)) {//自機下
				isDead = true;
				break;
			}
		}
		if (playerRect.top > 480 || playerRect.bottom < 0) {
			isDead = true;
		}
		DrawRotaGraph(bgPosX+240, 240,background_scale,0.0f, backH, true);//背景1
		DrawRotaGraph(bgPosX+240+480, 240,background_scale,0.0f, backH, true);//背景2
		DrawRotaGraph(bgPosX+240+480+480, 240,background_scale,0.0f, backH, true);//背景3
		//パイプの描画
		for(int i=0;i<8;++i){
			//上側パイプ描画
			DrawUpperPipe(pipePosX, background_scale, upperPipeRects[i], pipeH);
			//下側パイプ描画
			DrawLowerPipe(pipePosX, background_scale, lowerPipeRects[i], pipeH);

			//デバッグ用
			DrawBoxAA(upperPipeRects[i].left+pipePosX, upperPipeRects[i].top, upperPipeRects[i].right+pipePosX, upperPipeRects[i].bottom, 
						GetColor(255, 0, 0), false,2.0f);
			DrawBoxAA(lowerPipeRects[i].left+pipePosX, lowerPipeRects[i].top, lowerPipeRects[i].right+pipePosX, lowerPipeRects[i].bottom, 
						GetColor(255, 0, 0), false,2.0f);
		}
		DrawBoxAA(playerRect.left, playerRect.top, playerRect.right, playerRect.bottom, 
						GetColor(0, 255, 0), false,2.0f);

		//pipeStartIndexのパイプが画面外に出た
		if(upperPipeRects[pipeStartIndex].right+pipePosX<0){
			//最後のインデックスを計算
			int lastIndex = (pipeStartIndex + 7) % 8;//リングバッファのインデックスを更新
			
			//上側パイプ
			upperPipeRects[pipeStartIndex].left = upperPipeRects[lastIndex].left+horizontal_space_between_pipes;
			upperPipeRects[pipeStartIndex].top = 0;
			upperPipeRects[pipeStartIndex].right = upperPipeRects[lastIndex].right+horizontal_space_between_pipes;
			upperPipeRects[pipeStartIndex].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
			//下側パイプ
			lowerPipeRects[pipeStartIndex].left = lowerPipeRects[lastIndex].left+horizontal_space_between_pipes;
			lowerPipeRects[pipeStartIndex].top = ySpaceCenter + vertical_space_between_pipes / 2;
			lowerPipeRects[pipeStartIndex].right = lowerPipeRects[lastIndex].right+horizontal_space_between_pipes;
			lowerPipeRects[pipeStartIndex].bottom = 480;

			pipeStartIndex = (pipeStartIndex + 1) % 8;//リングバッファのインデックスを更新
			ySpaceCenter += (GetRand(40) - 20)*2;//パイプの中心位置を設定
		}

		//constexpr int vertical_space_between_pipes = 160;//パイプの上下の間隔
		//constexpr int horizontal_space_between_pipes = 80;//パイプの左右の間隔
		//for(int i=0;i<10;++i){
		//	//パイプの長さの計算
		//	int ySpaceCenter = 240;//パイプ間の通り抜けられるスペースの中心Y座標
		//	int pipeHeight = ySpaceCenter - vertical_space_between_pipes / 2;

		//	//パイプ上
		//	DrawUpperPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,0,
		//					background_scale, pipeHeight, pipeH);
		//	//パイプ下
		//	DrawLowerPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,480,
		//					background_scale, pipeHeight, pipeH);
		//}
		float angle = atan2f(vy, vx);//速度ベクトルを角度に変換
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);
		if(isDead){
			DrawString(200, 240, L"Game over press R!! for continue ", GetColor(255, 0, 0));
		}

		DrawFormatString(12, 12, GetColor(0,0,0), L"飛行距離=%d",-pipePosX);
		DrawFormatString(10, 10, GetColor(255, 255, 255), L"飛行距離=%d",-pipePosX);
		highScore = std::max(highScore, -pipePosX);
		DrawFormatString(212, 12, GetColor(0,0,0), L"最高飛行距離=%d",highScore);
		DrawFormatString(210, 10, GetColor(255, 255, 255), L"最高飛行距離=%d",highScore);

		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16ミリ秒経ってなかったら16ミリ秒になるまで待つ
			Sleep((unsigned int)(flip_interval-waitTime));
		}
		ScreenFlip();
	}
	return 0;
}