#include<DxLib.h>
#include<cassert>
#include<cmath>

#include<algorithm>

constexpr int edge_graph_height = 20;//�p�C�v�̒[�����̕����̌��摜�̍���
/// <summary>
/// ��`��\���\����
/// </summary>
struct Rect {
	int left;//��
	int top;//��
	int right;//�E
	int bottom;//��
};

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
/// ��`���m�̓����蔻��(��`�\���̔�)
/// </summary>
/// <param name="rcA">��`A</param>
/// <param name="rcB">��`B</param>
/// <param name="rcBOffsetX">��`B�̃I�t�Z�b�g</param>
/// <returns></returns>
bool IsHit(const Rect& rcA, const Rect& rcB,int rcBOffsetX=0) {
	return IsHit(rcA.left, rcA.right, rcA.top, rcA.bottom,
		rcB.left+rcBOffsetX, rcB.right+rcBOffsetX, rcB.top, rcB.bottom);
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
/// �㑤�p�C�v��`�悷��(��`�w���)
/// </summary>
/// <param name="offsetX">���SX���W</param>
/// <param name="drawScale">�`��X�P�[��</param>
/// <param name="rc">��`</param>
/// <param name="handle">�摜�n���h��</param>
void DrawUpperPipe(int offsetX,float drawScale,const Rect& rc,int handle){
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//����͑傫������(8�{������)�̂Ő؂�o��
	gw /= 4;
	gh /= 2;

	int left = rc.left + offsetX;//���[
	int right = rc.right+offsetX;//�E�[
	int top = rc.top;
	int bottom = rc.bottom - edge_graph_height*drawScale;//���[(�G�b�W�̎�O�܂�)
	//��G�b�W(�L�яk�݂��镔��)�`��
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//�G�b�W�ȊO�̕����̕`��
	//�G�b�W�`��
	DrawRectExtendGraph(left , bottom,right,rc.bottom,
		0, gh-edge_graph_height, gw, edge_graph_height, handle, true);
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

/// <summary>
/// �����p�C�v��`�悷��(��`�w���)
/// </summary>
/// <param name="offsetX">���SX���W</param>
/// <param name="drawScale">�`��X�P�[��</param>
/// <param name="rc">��`</param>
/// <param name="handle">�摜�n���h��</param>
void DrawLowerPipe(int offsetX,float drawScale,const Rect& rc,int handle){
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	//����͑傫������̂Ő؂�o��
	gw /= 4;
	gh /= 2;

	int left = rc.left + offsetX;//���[
	int right = rc.right+offsetX;//�E�[
	int top = rc.top + edge_graph_height*drawScale;//���[(�G�b�W�̎�O�܂�);
	int bottom = rc.bottom;//���[
	//��G�b�W(�L�яk�݂��镔��)�`��
	DrawRectExtendGraph(left, top, right, bottom,
		0, edge_graph_height, gw, gw, handle, true);//�G�b�W�ȊO�̕����̕`��
	//�G�b�W�`��
	DrawRectExtendGraph(left , rc.top,right,top,
		0, 0, gw, edge_graph_height, handle, true);
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

	Rect upperPipeRects[8]={};//�㑤�p�C�v�̋�`
	Rect lowerPipeRects[8]={};//�����p�C�v�̋�`
	int pipeStartIndex = 0;//�����O�o�b�t�@�p�̃p�C�v�̃C���f�b�N�X

	int ySpaceCenter = 240;//�p�C�v�Ԃ̒ʂ蔲������X�y�[�X�̒��SY���W
	constexpr int pipe_start_x = 320;//�p�C�v�̊J�nX���W
	constexpr int vertical_space_between_pipes = 200;//�p�C�v�̏㉺�̊Ԋu
	constexpr int horizontal_space_between_pipes = 200;//�p�C�v�̍��E�̊Ԋu
	//����̃p�C�v8�{�̋�`�𐶐�
	for (int i = 0; i < 8; ++i) {

		//�㑤�p�C�v
		upperPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
		upperPipeRects[i].top = 0;
		upperPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
		upperPipeRects[i].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
		//�����p�C�v
		lowerPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
		lowerPipeRects[i].top = ySpaceCenter + vertical_space_between_pipes / 2;
		lowerPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
		lowerPipeRects[i].bottom = 480;

		ySpaceCenter += (GetRand(40) - 20)*2;//�p�C�v�̏㉺�̊Ԋu
	}
	int highScore = 0;//�n�C�X�R�A
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
				pipeStartIndex = 0;
				ySpaceCenter = 240;
				//����̃p�C�v8�{�̋�`�𐶐�
				for (int i = 0; i < 8; ++i) {

					//�㑤�p�C�v
					upperPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
					upperPipeRects[i].top = 0;
					upperPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
					upperPipeRects[i].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
					//�����p�C�v
					lowerPipeRects[i].left = pipe_start_x+horizontal_space_between_pipes * i - 16 * background_scale;
					lowerPipeRects[i].top = ySpaceCenter + vertical_space_between_pipes / 2;
					lowerPipeRects[i].right = pipe_start_x+horizontal_space_between_pipes * i + 16 * background_scale;
					lowerPipeRects[i].bottom = 480;

					int diff = (GetRand(40) - 20)*2;
					ySpaceCenter += diff;//�p�C�v�̏㉺�̊Ԋu
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
			bgPosX -= vx;//�w�i�𑬓x�̋t�����ɃX�N���[��
			bgPosX = bgPosX % 480;//�w�i��480�s�N�Z�����ƂɌJ��Ԃ�
			pipePosX -= vx;//�w�i�𑬓x�̋t�����ɃX�N���[��
			//pipePosX = pipePosX % (int)((32 + 8) * background_scale);//
		}
		//�v���C���[�㉺���E
		Rect playerRect = {};
		playerRect.left = static_cast<int>(x - 6.0f * player_scale);//���@��
		playerRect.right = static_cast<int>(x + 6.0f * player_scale);//���@�E
		playerRect.top = static_cast<int>(y- 6.0f * player_scale);//���@��
		playerRect.bottom = static_cast<int>(y + 6.0f * player_scale);//���@��

		//�����蔻��
		for (int i = 0; i < 8; ++i) {
			//�p�C�v��Ɣ���
			if (IsHit(playerRect,upperPipeRects[i],pipePosX)) {//���@��
				isDead = true;
				break;
			}
			//�p�C�v���Ɣ���
			if (IsHit(playerRect,lowerPipeRects[i],pipePosX)) {//���@��
				isDead = true;
				break;
			}
		}
		if (playerRect.top > 480 || playerRect.bottom < 0) {
			isDead = true;
		}
		DrawRotaGraph(bgPosX+240, 240,background_scale,0.0f, backH, true);//�w�i1
		DrawRotaGraph(bgPosX+240+480, 240,background_scale,0.0f, backH, true);//�w�i2
		DrawRotaGraph(bgPosX+240+480+480, 240,background_scale,0.0f, backH, true);//�w�i3
		//�p�C�v�̕`��
		for(int i=0;i<8;++i){
			//�㑤�p�C�v�`��
			DrawUpperPipe(pipePosX, background_scale, upperPipeRects[i], pipeH);
			//�����p�C�v�`��
			DrawLowerPipe(pipePosX, background_scale, lowerPipeRects[i], pipeH);

			//�f�o�b�O�p
			DrawBoxAA(upperPipeRects[i].left+pipePosX, upperPipeRects[i].top, upperPipeRects[i].right+pipePosX, upperPipeRects[i].bottom, 
						GetColor(255, 0, 0), false,2.0f);
			DrawBoxAA(lowerPipeRects[i].left+pipePosX, lowerPipeRects[i].top, lowerPipeRects[i].right+pipePosX, lowerPipeRects[i].bottom, 
						GetColor(255, 0, 0), false,2.0f);
		}
		DrawBoxAA(playerRect.left, playerRect.top, playerRect.right, playerRect.bottom, 
						GetColor(0, 255, 0), false,2.0f);

		//pipeStartIndex�̃p�C�v����ʊO�ɏo��
		if(upperPipeRects[pipeStartIndex].right+pipePosX<0){
			//�Ō�̃C���f�b�N�X���v�Z
			int lastIndex = (pipeStartIndex + 7) % 8;//�����O�o�b�t�@�̃C���f�b�N�X���X�V
			
			//�㑤�p�C�v
			upperPipeRects[pipeStartIndex].left = upperPipeRects[lastIndex].left+horizontal_space_between_pipes;
			upperPipeRects[pipeStartIndex].top = 0;
			upperPipeRects[pipeStartIndex].right = upperPipeRects[lastIndex].right+horizontal_space_between_pipes;
			upperPipeRects[pipeStartIndex].bottom = ySpaceCenter - vertical_space_between_pipes / 2;
			//�����p�C�v
			lowerPipeRects[pipeStartIndex].left = lowerPipeRects[lastIndex].left+horizontal_space_between_pipes;
			lowerPipeRects[pipeStartIndex].top = ySpaceCenter + vertical_space_between_pipes / 2;
			lowerPipeRects[pipeStartIndex].right = lowerPipeRects[lastIndex].right+horizontal_space_between_pipes;
			lowerPipeRects[pipeStartIndex].bottom = 480;

			pipeStartIndex = (pipeStartIndex + 1) % 8;//�����O�o�b�t�@�̃C���f�b�N�X���X�V
			ySpaceCenter += (GetRand(40) - 20)*2;//�p�C�v�̒��S�ʒu��ݒ�
		}

		//constexpr int vertical_space_between_pipes = 160;//�p�C�v�̏㉺�̊Ԋu
		//constexpr int horizontal_space_between_pipes = 80;//�p�C�v�̍��E�̊Ԋu
		//for(int i=0;i<10;++i){
		//	//�p�C�v�̒����̌v�Z
		//	int ySpaceCenter = 240;//�p�C�v�Ԃ̒ʂ蔲������X�y�[�X�̒��SY���W
		//	int pipeHeight = ySpaceCenter - vertical_space_between_pipes / 2;

		//	//�p�C�v��
		//	DrawUpperPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,0,
		//					background_scale, pipeHeight, pipeH);
		//	//�p�C�v��
		//	DrawLowerPipe(pipePosX + (32 + 8) * background_scale * i + 24 * background_scale,480,
		//					background_scale, pipeHeight, pipeH);
		//}
		float angle = atan2f(vy, vx);//���x�x�N�g�����p�x�ɕϊ�
		DrawRectRotaGraphF(320.0f, y,0,0,16,16,3.0f,angle, playerH, true);
		if(isDead){
			DrawString(200, 240, L"Game over press R!! for continue ", GetColor(255, 0, 0));
		}

		DrawFormatString(12, 12, GetColor(0,0,0), L"��s����=%d",-pipePosX);
		DrawFormatString(10, 10, GetColor(255, 255, 255), L"��s����=%d",-pipePosX);
		highScore = std::max(highScore, -pipePosX);
		DrawFormatString(212, 12, GetColor(0,0,0), L"�ō���s����=%d",highScore);
		DrawFormatString(210, 10, GetColor(255, 255, 255), L"�ō���s����=%d",highScore);

		auto waitTime=GetTickCount64() - lastTime;
		if(waitTime<flip_interval){//16�~���b�o���ĂȂ�������16�~���b�ɂȂ�܂ő҂�
			Sleep((unsigned int)(flip_interval-waitTime));
		}
		ScreenFlip();
	}
	return 0;
}