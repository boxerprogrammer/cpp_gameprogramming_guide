
#include<DxLib.h>

#include"../Application.h"
#include"../Input.h"

#include "PauseScene.h"
#include"SceneController.h"
#include"KeyconfigScene.h"
#include"SoundconfigScene.h"
#include"TitleScene.h"

constexpr int margin_size = 50;
constexpr int appear_interval = 15;//ポーズ画面出現時間

//表示されるメニュー用定数
constexpr int menu_top = 100;//メニュー開始位置(上)
constexpr int menu_left = 100;//メニュー開始位置(左)
constexpr int menu_item_height = 30;//メニュー一行当たりの高さ

//Yes/Noダイアログの幅と高さ
constexpr int yesno_dialog_width = 300;
constexpr int yesno_dialog_height = 150;
constexpr int yesno_dialog_left = 80;
constexpr int yesno_dialog_top = 70;
constexpr int yesno_text_width = 80;

constexpr int arrow_offset = -30;

constexpr int yesno_yes = 0;
constexpr int yesno_no = 1;

void PauseScene::AppearUpdate(Input&)
{
	if (++frame_ >= appear_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		frame_ = 0;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::FrameOnlyDraw;
		frame_ = appear_interval;
		return;
	}
	if (input.IsTriggered("up")) {
		currentIndex_=static_cast<int>((currentIndex_+ menuItems_.size() -1) % menuItems_.size());
	}
	if (input.IsTriggered("down")) {
		currentIndex_= (currentIndex_+1)%menuItems_.size();
	}
	//OKをおしたときに選択中のコマンドを実行する
	if (input.IsTriggered("ok")) {
		//中身は気にせず実行する
		menuFunctions_[menuItems_[currentIndex_]](input);
		return;
	}
}

void PauseScene::YesNoUpdate(Input& input)
{
	if (input.IsTriggered("right")) {
		yesNoIndex_ = std::min(yesNoIndex_ + 1, static_cast<int>(yesNoItems_.size()-1));
	}
	if (input.IsTriggered("left")) {
		yesNoIndex_ = std::max(yesNoIndex_ - 1, 0);
	}
	if (input.IsTriggered("ok")) {
		if (yesNoIndex_ == yesno_yes) {//はい
			deferredExec_();
			return;
		}
		else {
			update_ = &PauseScene::NormalUpdate;
			draw_ = &PauseScene::NormalDraw;
		}
	}
}

void PauseScene::DisappearUpdate(Input&)
{
	if(--frame_<=0){
		controller_.PopScene();
		return;
	}
	
}

void PauseScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);//半透明描画にする
	DrawBoxAA(
		static_cast<float>(margin_size),
		static_cast<float>(margin_size),
		static_cast<float>(wsize.w - margin_size),
		static_cast<float>(wsize.h - margin_size),
		0x8888aa,
		true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
	//枠の描画
	DrawBoxAA(
		static_cast<float>(margin_size),
		static_cast<float>(margin_size),
		static_cast<float>(wsize.w - margin_size),
		static_cast<float>(wsize.h - margin_size),
		0xffffff,
		false,
		3.0f
	);
	//テキストの描画
	DrawString(0 + margin_size + 10,
		0 + margin_size + 10,
		L"Pause Scene...",
		0xffffff);

	
	int menuY = margin_size + menu_top;
	

	for (auto& item : menuItems_) {
		auto itemColor = 0xddddff;
		auto shadowColor = 0x0000aa;
		int menuX = margin_size + menu_left;
		//今、選択中の行ですよ
		if (menuItems_[currentIndex_] == item) {
			itemColor = 0xffdddd;
			shadowColor = 0xaa0000;
			
			DrawString(menuX + 2+ arrow_offset, menuY + 2,L"☞", shadowColor);//影
			DrawString(menuX+ arrow_offset, menuY, L"☞", itemColor);//本体
			menuX += 5;//メニューアイテムを右に少しだけずらす
		}

		//影文字を表示
		DrawFormatString(menuX + 2, menuY + 2, shadowColor, L"%s", item.c_str());//影
		DrawFormatString(menuX, menuY, itemColor, L"%s", item.c_str());//本体
		menuY += menu_item_height;
	}

}

void PauseScene::FrameOnlyDraw()
{
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);

	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto ycenter = wsize.h / 2;//枠の真ん中の座標を計算
	const auto max_height = wsize.h / 2 - margin_size;//最大高さ÷2
	const auto current_height = static_cast<float>(max_height) * rate;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);//半透明描画にする
	DrawBoxAA(
		static_cast<float>(margin_size),
		static_cast<float>(ycenter) - current_height,//上端
		static_cast<float>(wsize.w - margin_size),
		static_cast<float>(ycenter) + current_height,//下端
		0x8888aa,
		true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
	//枠の描画
	DrawBoxAA(
		static_cast<float>(margin_size),
		static_cast<float>(ycenter) - current_height,//上端
		static_cast<float>(wsize.w - margin_size),
		static_cast<float>(ycenter) + current_height,//下端
		0xffffff,
		false,
		3.0f
	);
}

void PauseScene::YesNoDraw()
{
	NormalDraw();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	int centerX = wsize.w / 2;
	int centerY = wsize.h / 2;
	//面
	DrawBox(centerX - yesno_dialog_width / 2,
			centerY - yesno_dialog_height / 2,
			centerX + yesno_dialog_width / 2,
			centerY + yesno_dialog_height / 2,
			0x8888888,true);
	//枠
	DrawBox(centerX - yesno_dialog_width / 2,
		centerY - yesno_dialog_height / 2,
		centerX + yesno_dialog_width / 2,
		centerY + yesno_dialog_height / 2,
		0xffffff, false,3);

	constexpr int yesno_margin = 10;
	int x = yesno_margin+centerX - yesno_dialog_width / 2;
	int y = yesno_margin+centerY - yesno_dialog_height / 2;
	DrawFormatString(x+2, y+2, 0x000000, L"%s？", menuItems_[currentIndex_].c_str());
	DrawFormatString(x, y, 0xffffff, L"%s？", menuItems_[currentIndex_].c_str());

	x += yesno_dialog_left- yesno_margin;
	y += yesno_dialog_top- yesno_margin;


	for (auto& item : yesNoItems_) {
		auto textColor = 0xffffff;
		if (item == yesNoItems_[yesNoIndex_]) {
			textColor = 0xaaffaa;
			DrawString(x + 2 + arrow_offset, y + 2, L"☞", 0x000000);//影
			DrawString(x+arrow_offset, y,  L"☞", textColor);//本体
		}
		DrawFormatString(x + 2, y + 2, 0x000000, L"%s", item.c_str());//影
		DrawFormatString(x, y, textColor, L"%s", item.c_str());//本体
		x += yesno_text_width;
	}

}

PauseScene::PauseScene(SceneController& ctrl):Scene(ctrl)
{
	update_ = &PauseScene::AppearUpdate;//出現
	draw_ = &PauseScene::FrameOnlyDraw;
	frame_ = 0;
	menuItems_ = {
		L"ゲームに戻る",
		L"キーコンフィグ",
		L"サウンド設定",
		L"タイトルに戻る",
		L"ゲームを終了する"
	};

	menuFunctions_[L"ゲームに戻る"] = [this](Input&) {
		//「ゲームに戻る」を押されたら、ポーズメニューを閉じる
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::FrameOnlyDraw;
		frame_ = appear_interval;
		};
		
	menuFunctions_[L"キーコンフィグ"] = [this](Input& input) {
		controller_.PushScene(std::make_shared<KeyconfigScene>(controller_,input));
		};
	menuFunctions_[L"サウンド設定"] = [this](Input&) {
		controller_.PushScene(std::make_shared<SoundconfigScene>(controller_));
		};
	menuFunctions_[L"タイトルに戻る"] = [this](Input&) {
			deferredExec_ = [this]() {
				controller_.ResetScene(std::make_shared<TitleScene>(controller_));
			};
			update_ = &PauseScene::YesNoUpdate;
			draw_ = &PauseScene::YesNoDraw;
		};
	menuFunctions_[L"ゲームを終了する"] = [this](Input&) {
			deferredExec_ = []() {
				auto& app = Application::GetInstance();
				app.RequestExit();
			};
			update_ = &PauseScene::YesNoUpdate;
			draw_ = &PauseScene::YesNoDraw;
		};
	yesNoItems_ = { L"はい",L"いいえ" };
}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}
