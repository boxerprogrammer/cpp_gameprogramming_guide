#include<DxLib.h>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int playerH = LoadGraph(L"../img/bird.png");

	while(ProcessMessage()!=-1){
		ClearDrawScreen();
		//DrawGraph(320, 240,playerH, true);//�����̕\��
		//DrawRectGraph(320, 240,0,0,16,16, playerH, true);//�ꕔ�����\��
		
		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		//�ꕔ���g�債�ĕ\��
		DrawRectRotaGraph(320, 240,//�\���ꏊ
							0,0,//�摜�̍���
							16,16,//�摜�̕��A����
							3.0f, 0.0f, //�g�嗦�A��]�p�x
							playerH, true);//�摜�n���h���A���߂��邩�ǂ���
		ScreenFlip();
	}
	return 0;
}