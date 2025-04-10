#include<DxLib.h>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int playerH = LoadGraph(L"../img/bird.png");

	while(ProcessMessage()!=-1){
		ClearDrawScreen();
		//DrawGraph(320, 240,playerH, true);//ただの表示
		//DrawRectGraph(320, 240,0,0,16,16, playerH, true);//一部だけ表示
		
		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		//一部を拡大して表示
		DrawRectRotaGraph(320, 240,//表示場所
							0,0,//画像の左上
							16,16,//画像の幅、高さ
							3.0f, 0.0f, //拡大率、回転角度
							playerH, true);//画像ハンドル、透過するかどうか
		ScreenFlip();
	}
	return 0;
}