#include "TitleScene.h"
#include<DxLib.h>
#include"../Input.h"
#include"GameScene.h"
#include"SceneController.h"
#include"../Application.h"

#include<array>
#include<cassert>

constexpr int fade_interval = 60;


namespace mylib {
	void DrawGraph(int x, int y ,int handle, int psH,int secondH=-1,bool isAlpha=false) {
		int gw, gh;
		GetGraphSize(handle, &gw, &gh);//画像のサイズを取得する
		std::array<VERTEX2DSHADER,4> vertices;//頂点の複数形
		for (auto& v : vertices) {
			v.rhw = 1.0f;//あとで説明します(同次座標系のw)
			v.dif = GetColorU8(255, 255, 255, 255);//基本色(テクスチャがある場合は白でいい)
			v.spc = GetColorU8(0, 0, 0, 0);//スペキュラー(無視でOK)
			v.pos.z = 0.0f;//2Dでやってるので0でいい
		}
		//４頂点定義(Zの形になるように)
		//左上
		vertices[0].pos.x = x;
		vertices[0].pos.y = y;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//右上
		vertices[1].pos.x = x+gw;
		vertices[1].pos.y = y;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//左下
		vertices[2].pos.x = x;
		vertices[2].pos.y = y+gh;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//右
		vertices[3].pos.x = x + gw;
		vertices[3].pos.y = y + gh;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		SetUsePixelShader(psH);//どのシェーダを使用するか
		SetUseTextureToShader(0, handle);//描画に使用するテクスチャの指定
		if (secondH >= 0) {
			SetUseTextureToShader(1, secondH);
		}
		if (isAlpha) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		//ひとまずTRIANGLESTRIPを指定して頂点座標の配列を渡します。
		DrawPrimitive2DToShader(vertices.data(), vertices.size(), DX_PRIMTYPE_TRIANGLESTRIP);
		SetUsePixelShader(-1);
		SetUseTextureToShader(0, -1);
		SetUseTextureToShader(1, -1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void TitleScene::FadeInUpdate(Input&)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	if (--fadeFrame_ <= 0) {
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	++frame_;
	
	if (input.IsTriggered("ok")) {
		PlaySoundMem(startSE_, DX_PLAYTYPE_BACK);
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		fadeFrame_ = 0;//フェードアウトの最初
		return;
	}


	float sec = static_cast<float>(GetTickCount()%1000)/1000.0f;
	float rad = sec * DX_PI_F * 2.0f;
	pCBuff_->lx = cos(rad);
	pCBuff_->ly = sin(rad);
	pCBuff_->lz = 1.0;
	UpdateShaderConstantBuffer(cBuffH_);//GPU上に転送
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (++fadeFrame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;
	}
}

void TitleScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawGraph(0, 0, backH_, false);
	//DrawRotaGraph(wsize.w/2, wsize.h/2, 1.0f, 0.0f, titleH_, true);
	SetShaderConstantBuffer(cBuffH_, DX_SHADERTYPE_PIXEL, 4);
	mylib::DrawGraph(100, 100, titleH_, psNormH_,titleNormalH_,true);
	//DrawRotaGraph(wsize.w/2, wsize.h/2, 0.75f, 0.0f, titleLogoH_, true);

	if ((frame_ / 30) % 2 == 0) {
		//press start
		const wchar_t* press_start = L"Press Start Button";
		int fontSize = GetFontSizeToHandle(titleFontH_);
		int strW = GetDrawStringWidthToHandle(press_start, wcslen(press_start), titleFontH_);
		int y = (wsize.h - fontSize) / 2;
		DrawShadowedString((wsize.w - strW) / 2, y, press_start, 0xffff00);
		const wchar_t* press_start_jp = L"スタートボタンを押してね";
		strW = GetDrawStringWidthToHandle(press_start_jp, wcslen(press_start_jp), titleFontH_);
		y += fontSize;
		DrawShadowedString((wsize.w - strW) / 2, y, press_start_jp, 0xffff00);
	}


}

void TitleScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, titleH_, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, titleLogoH_, true);
	//値の範囲をいったん0.0～1.0にしておくと色々と扱いやすくなります
	auto rate = static_cast<float>(fadeFrame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);//αブレンド
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);//画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);//ブレンドしない
}

void TitleScene::DrawShadowedString(int x, int y ,const wchar_t* wstr, unsigned int color)
{
	constexpr int text_offset = 2;
	
	DrawStringToHandle(x + text_offset,
		y + text_offset
		, wstr, 0x888888, titleFontH_);
	DrawStringToHandle(x, y, wstr, color, titleFontH_);
}

TitleScene::TitleScene(SceneController& controller):Scene(controller),
fadeFrame_(fade_interval),
update_(&TitleScene::FadeInUpdate)
{
	titleH_ = LoadGraph(L"img/title_bg.png");
	titleNormalH_ = LoadGraph(L"img/title_bg_n.png");
	assert(titleNormalH_ >= 0);
	titleLogoH_ = LoadGraph(L"img/game_title.png");
	draw_ = &TitleScene::FadeDraw;
	fadeFrame_ = fade_interval;
	titleFontH_ = LoadFontDataToHandle(L"font/FontData.dft");
	startSE_ = LoadSoundMem(L"se/start.wav");

	cloudH_ = LoadGraph(L"img/cloud.png");
	assert(cloudH_ >= 0);


	backH_ = LoadGraph(L"img/title_back.jpg");
	assert(backH_ >= 0);

	psH_ = LoadPixelShader(L"FirstShader.pso");
	assert(psH_ >= 0);

	psNormH_ = LoadPixelShader(L"NormalMap.pso");
	assert(psNormH_ >= 0);


	cBuffH_=CreateShaderConstantBuffer(sizeof(ConstantBuffer));//GPU上にメモリを確保
	pCBuff_ = static_cast<ConstantBuffer*>(GetBufferShaderConstantBuffer(cBuffH_));//双子バッファのアドレス
	pCBuff_->threshold = 0.0f;
}

TitleScene::~TitleScene()
{
	DeleteGraph(titleH_);
	DeleteGraph(titleLogoH_);
	DeleteFontToHandle(titleFontH_);
	DeleteSoundMem(startSE_);

}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);//現在割り当てられているUpdate系メンバ関数を実行する
}

void TitleScene::Draw()
{
	(this->*draw_)();//割り当てられているDraw系メンバ関数を実行する
}
