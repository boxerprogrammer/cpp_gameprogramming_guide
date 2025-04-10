#include<DxLib.h>
#include<cassert>
#include<cmath>
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int playerH = LoadGraph(L"../img/bird.png");

	float y = 240.0f;//���@��Y���W
	float vy = 0.0f;//���@��Y�������x
	float vx = 3.0f;//���@��X�������x
	constexpr float g = 0.3f;//�d�͉����x
	constexpr float jump_velocity = -6.0f;//�W�����v�̏u�ԑ��x(�����)

	//�Q�[���X�s�[�h�����ɕۂ��߂̍H�v
	auto lastTime = GetTickCount64();//���O�̃~���b���v���Ă���
	constexpr auto flip_interval = 16U;//��ʍX�V������̃~���b(1000/60=16.66����16�~���b�Ƃ���)

	auto lastMouseSate = GetMouseInput();
	
	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//�N���b�N���ꂽ��󒆃W�����v����
		auto currentMouseState = GetMouseInput();
		if(!(lastMouseSate&MOUSE_INPUT_LEFT) && (currentMouseState&&MOUSE_INPUT_LEFT)){
			vy = jump_velocity;
		}
		vy += g;
		y += vy;


		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		float angle = atan2f(vy, vx);//���x�x�N�g�����p�x�ɕϊ�
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);

		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16�~���b�o���ĂȂ�������16�~���b�ɂȂ�܂ő҂�
			Sleep((unsigned int)waitTime);
		}
		ScreenFlip();
	}
	return 0;
}