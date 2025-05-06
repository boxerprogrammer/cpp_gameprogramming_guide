#include<DxLib.h>
#include<cassert>
#include<cmath>

#include<algorithm>

/// <summary>
/// ��`���m�̓����蔻��
/// </summary>
/// <param name="leftA">A�̍�</param>
/// <param name="rightA">A�̉E</param>
/// <param name="topA">A�̏�</param>
/// <param name="bottomA">A�̉�</param>
/// <param name="leftB">B�̍�</param>
/// <param name="rightB">B�̉E</param>
/// <param name="topB">B�̏�</param>
/// <param name="bottomB">B�̉�</param>
/// <returns>true:�������Ă��� / false:�������Ă��Ȃ�</returns>
bool IsHit(int leftA, int rightA, int topA, int bottomA,
	int leftB, int rightB, int topB, int bottomB) {
	return !(std::min(rightA, rightB) < std::max(leftA, leftB) ||
		std::min(bottomA, bottomB) < std::max(topA, topB));
}


/// <summary>
/// �㑤�p�C�v��`�悷��
/// </summary>
/// <param name="x">���SX���W</param>
/// <param name="topY">��[(������Œ�)</param>
/// <param name="drawScale">�`��X�P�[��(�p�C�v�̒����Ƃ͕�)</param>
/// <param name="length">���[����̃p�C�v�̒���</param>
/// <param name="handle">�摜�n���h��</param>
void DrawUpperPipe(int x,int topY,float drawScale,int length,int handle){
	constexpr int edge_graph_height = 20;//�p�C�v�̒[�����̕����̌��摜�̍���
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//����͑傫������̂Ő؂�o��
	gw /= 4;
	gh /= 2;

	int left = x-gw * drawScale/2;//���[
	int right = x + gw * drawScale / 2;//�E�[
	int top = topY;
	int bottom = topY + length - edge_graph_height*drawScale;//���[(�G�b�W�̎�O�܂�)
	//��G�b�W(�L�яk�݂��镔��)�`��
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//�G�b�W�ȊO�̕����̕`��
	//�G�b�W�`��
	DrawRectRotaGraph(x, bottom - edge_graph_height * drawScale/2,
		0, gh-edge_graph_height, gw, edge_graph_height, 
		drawScale,0.0f,handle, true);//�G�b�W�ȊO�̕����̕`��

	

}

/// <summary>
/// �����p�C�v��`�悷��
/// </summary>
/// <param name="x">���SX���W</param>
/// <param name="bottomY">���[Y(������Œ�)</param>
/// <param name="drawScale">�`��X�P�[��(�p�C�v�̒����Ƃ͕�)</param>
/// <param name="length">���[����̃p�C�v�̒���</param>
/// <param name="handle">�摜�n���h��</param>
void DrawLowerPipe(int x,int bottomY,float drawScale,int length,int handle){
	constexpr int edge_graph_height = 20;//�p�C�v�̒[�����̕����̌��摜�̍���
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//����͑傫������̂Ő؂�o��
	gw /= 4;
	gh /= 2;

	int left = x-gw * drawScale/2;//���[
	int right = x + gw * drawScale / 2;//�E�[
	int top = bottomY - length + edge_graph_height * drawScale;//��[(�G�b�W�̎�O�܂�)
	int bottom = bottomY;
	//��G�b�W(�L�яk�݂��镔��)�`��
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//�G�b�W�ȊO�̕����̕`��
	//�G�b�W�`��
	DrawRectRotaGraph(x, bottom-length+edge_graph_height*drawScale/2,
		0, 0, gw, edge_graph_height, 
		drawScale,0.0f,handle, true);//�G�b�W�ȊO�̕����̕`��

	DrawBoxAA(left, top, right, bottom, GetColor(255, 0, 0),false,2.0f);//�f�o�b�O�p
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	ChangeWindowMode(true);
	if(DxLib_Init()==-1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int playerH = LoadGraph(L"../img/bird.png");
	int backH = LoadGraph(L"../img/Background2.png");//�w�i�̃��[�h
	int pipeH = LoadGraph(L"../img/PipeStyle1.png");//�p�C�v�̃��[�h

	float x = 320.0f;//���@��X���W
	float y = 240.0f;//���@��Y���W
	float vx = 3.0f;//���@��X�������x
	float vy = 0.0f;//���@��Y�������x
	constexpr float player_scale = 3.0f;//���@�̊g�嗦
	constexpr float g = 0.3f;//�d�͉����x
	constexpr float jump_velocity = -6.0f;//�W�����v�̏u�ԑ��x(�����)
	constexpr float background_scale = 480.0f / 256.0f;//�w�i�g�嗦


	//�Q�[���X�s�[�h�����ɕۂ��߂̍H�v
	auto lastTime = GetTickCount64();//���O�̃~���b���v���Ă���
	constexpr auto flip_interval = 16U;//��ʍX�V������̃~���b(1000/60=16.66����16�~���b�Ƃ���)

	auto lastMouseSate = GetMouseInput();
	int bgPosX = 0;//�w�i�̊X���W
	int pipePosX = 0;//�p�C�v�̊X���W
	
	bool isDead = false;

	while(ProcessMessage()!=-1){
		ClearDrawScreen();

		//�N���b�N���ꂽ��󒆃W�����v����
		auto currentMouseState = GetMouseInput();

		if (isDead) {
			
			if (CheckHitKey(KEY_INPUT_R)) {
				isDead = false;
				x = 320.0f;//���@��X���W
				y = 240.0f;//���@��Y���W
				vy = 0.0f;//���@��Y�������x
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
			bgPosX -= vx;//�w�i�𑬓x�̋t�����ɃX�N���[��
			bgPosX = bgPosX % 480;//�w�i��480�s�N�Z�����ƂɌJ��Ԃ�
			pipePosX -= vx;//�w�i�𑬓x�̋t�����ɃX�N���[��
			pipePosX = pipePosX % (int)((32 + 8) * background_scale);//
		}
		//�v���C���[�㉺���E
		int playerL = static_cast<int>(x - 8.0f * player_scale);//���@��
		int playerR = static_cast<int>(x + 8.0f * player_scale);//���@�E
		int playerT = static_cast<int>(y- 8.0f * player_scale);//���@��
		int playerB = static_cast<int>(y + 8.0f * player_scale);//���@��

		//�����蔻��
		for (int i = 0; i < 10; ++i) {
			//�p�C�v��Ɣ���
			int upperPipeCenterX = pipePosX + (32 + 8) * background_scale * i + 24 * background_scale;
			int upperPipeCenterY = 0;
			if (IsHit(
				upperPipeCenterX - 16 * background_scale,//�p�C�v��
				upperPipeCenterX + 16 * background_scale,//�p�C�v�E
				upperPipeCenterY - 40 * background_scale,//�p�C�v��
				upperPipeCenterY + 40 * background_scale,//�p�C�v��
				playerL,//���@��
				playerR,//���@�E
				playerT,//���@��
				playerB)) {//���@��
				isDead = true;
				break;
			}

			//�p�C�v���Ɣ���
			int lowerPipeCenterX = pipePosX + (32 + 8) * background_scale * i + 24 * background_scale;
			int lowerPipeCenterY = 480;
			if (IsHit(
				lowerPipeCenterX - 16 * background_scale,//�p�C�v��
				lowerPipeCenterX + 16 * background_scale,//�p�C�v�E
				lowerPipeCenterY - 40 * background_scale,//�p�C�v��
				lowerPipeCenterY + 40 * background_scale,//�p�C�v��
				playerL,//���@��
				playerR,//���@�E
				playerT,//���@��
				playerB)) {//���@��
				isDead = true;
				break;
			}
		}



		DrawBox(0, 0, 640, 480, GetColor(64, 128, 255), true);
		DrawRotaGraph(bgPosX+240, 240,background_scale,0.0f, backH, true);//�w�i1
		DrawRotaGraph(bgPosX+240+480, 240,background_scale,0.0f, backH, true);//�w�i2
		DrawRotaGraph(bgPosX+240+480+480, 240,background_scale,0.0f, backH, true);//�w�i3
		constexpr int vertical_space_between_pipes = 160;//�p�C�v�̏㉺�̊Ԋu
		constexpr int horizontal_space_between_pipes = 80;//�p�C�v�̍��E�̊Ԋu
		for(int i=0;i<10;++i){
			//�p�C�v�̒����̌v�Z
			int ySpaceCenter = 240;//�p�C�v�Ԃ̒ʂ蔲������X�y�[�X�̒��SY���W
			int pipeHeight = ySpaceCenter - vertical_space_between_pipes / 2;

			//�p�C�v��
			DrawUpperPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,0,
							background_scale, pipeHeight, pipeH);
			//�p�C�v��
			DrawLowerPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,480,
							background_scale, pipeHeight, pipeH);
		}
		float angle = atan2f(vy, vx);//���x�x�N�g�����p�x�ɕϊ�
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);
		if(isDead){
			DrawString(200, 240, L"Game over press R!! for continue ", GetColor(255, 0, 0));
		}
		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16�~���b�o���ĂȂ�������16�~���b�ɂȂ�܂ő҂�
			Sleep((unsigned int)(flip_interval-waitTime));
		}
		ScreenFlip();
	}
	return 0;
}