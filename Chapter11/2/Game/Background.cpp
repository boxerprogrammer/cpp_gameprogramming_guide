#include "Background.h"
#include<DxLib.h>
#include"../Application.h"
#include"../FileLoader.h"

constexpr int planet_index = 1;

Background::Background()
{
	auto loader = Application::GetInstance().GetLoader();
	bgParts_.resize(3);
	bgParts_[0].handle = loader->LoadGraph(L"img/BackGround/bg-back.png");
	bgParts_[1].handle = loader->LoadGraph(L"img/BackGround/bg-stars.png");
	bgParts_[2].handle = loader->LoadGraph(L"img/BackGround/bg-planet.png");

	constexpr float regular_scroll_speed = 10.0f;
	bgParts_[0].scrollSpeeds = -regular_scroll_speed*0.125f;//最奥
	bgParts_[1].scrollSpeeds = -regular_scroll_speed*0.5f;//中間
	bgParts_[2].scrollSpeeds = -regular_scroll_speed;//手前

	bgParts_[2].flip = true;
}

void Background::Update()
{
	for (auto& bgPart : bgParts_) {
		bgPart.pos.x += bgPart.scrollSpeeds;
	}
}

void Background::Draw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto drawWidth = wsize.w;
	//背景をまとめて表示する
	for (auto& bgPart : bgParts_) {
		//%で繰り返しを行う。幅と同じ大きさになった時に0になる
		int x;
		if (bgPart.flip) {
			x = static_cast<int>(bgPart.pos.x) % (drawWidth*2);//フリップで二枚組なので長さが２倍
		}
		else{
			x = static_cast<int>(bgPart.pos.x) % drawWidth;
		}
		//必要な枚数(1)と
		DrawExtendGraph(x, wsize.h,
						x+ drawWidth, 0,
						bgPart.handle,true);
		if (bgPart.flip) {//フリップするため２枚組にしておく
			//プラス１
			DrawExtendGraph(x + drawWidth*1, 0,
							x + drawWidth *2, wsize.h,
				bgPart.handle, true);
			//プラス２
			DrawExtendGraph(x + drawWidth * 2, wsize.h,
							x + drawWidth * 3, 0,
				bgPart.handle, true);
		}
		else {
			//プラス１
			DrawExtendGraph(x + drawWidth, wsize.h,
				x + drawWidth + drawWidth, 0,
				bgPart.handle, true);
		}
	}
}
