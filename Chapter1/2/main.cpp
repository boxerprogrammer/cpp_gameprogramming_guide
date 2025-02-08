#include<DxLib.h>
#include<cassert>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int playerH = LoadGraph(L"../img/Abeja.png");

	float y = 240.0f;//���@��Y���W
	float vy = 0.0f;//���@��Y�������x
	constexpr float g = 0.3f;//�d�͉����x
	constexpr float jump_velocity = -6.0f;//�W�����v�̏u�ԑ��x(�����)

	//�Q�[���X�s�[�h�����ɕۂ��߂̍H�v
	auto lastTime = GetTickCount64();//���O�̃~���b���v���Ă���
	constexpr auto flip_interval = 16U;//��ʍX�V������̃~���b(1000/60=16.66����16�~���b�Ƃ���)

	auto lastMouseSate = GetMouseInput();

	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//�N���b�N���ꂽ��󒆃W�����v����(�҂��)
		auto currentMouseState = GetMouseInput();
		if(!(lastMouseSate&MOUSE_INPUT_LEFT) && (currentMouseState&&MOUSE_INPUT_LEFT)){
			vy = jump_velocity;
		}

		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		DrawRectRotaGraphF(320.0f, y,0,0,240,240,0.3f,0.0f, playerH, true);
		vy += g;
		y += vy;
		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16�~���b�o���ĂȂ�������16�~���b�ɂȂ�܂ő҂�
			Sleep((unsigned int)waitTime);
		}
		ScreenFlip();
	}
	return 0;
}