#include "PauseScene.h"
#include"../Input.h"
#include"SceneController.h"
#include"../Application.h"
#include"KeyConfigScene.h"
#include"TitleScene.h"
#include<DxLib.h>

constexpr int frame_margin = 10;//枠が画面端からどれくらい離れてるか
constexpr int appear_interval = 10;//枠が出現するまでのフレーム数
constexpr int menu_row_height = 50;//メニューの行の高さ
constexpr int menu_left_margin = 200;//メニュー枠からの左余白
constexpr int menu_top_margin = 120;//メニュー枠からの上余白
constexpr int yes_no_dialog_yes = 0;//YesNoDialogでYes
constexpr int yes_no_dialog_no = 1;//YesNoDialogでNo

void PauseScene::AppearUpdate(Input& input)
{
	if (frame_ == appear_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
	++frame_;
}
void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::IntervalDraw;
		frame_ = appear_interval;
		return;
	}
	if (input.IsTriggered("up")) {
		selectIndex_ = (selectIndex_+ menuList_.size() - 1) % menuList_.size();
	}
	if (input.IsTriggered("down")) {
		selectIndex_ = (selectIndex_+1)%menuList_.size();
	}
	if (input.IsTriggered("ok")) {
		//現在選択中のメニューアイテム名を取得する
		auto& menuName = menuList_[selectIndex_];
		//そのメニューアイテムの名前に対応付けられたラムダ式を実行する
		execTable_[menuName](input);
		return;
	}
}
void PauseScene::DisappearUpdate(Input& input)
{
	if (frame_ == 0) {
		controller_.PopScene();//この時点で自分は解放されてる
		return;
	}
	--frame_;
}

void PauseScene::YesNoDialogUpdate(Input& input)
{
	if (input.IsTriggered("left") || input.IsTriggered("right")) {
		yesNoIndex_ = (yesNoIndex_ + 1) % 2;
		return;
	}
	if (input.IsTriggered("ok")) {
		//YESが選ばれた
		if (yesNoIndex_ == yes_no_dialog_yes) {
			yesRequestFunction_();
		}
		else {//NOが選ばれた
			yesRequestFunction_ = []() {};//念のためクリア
			update_ = &PauseScene::NormalUpdate;
			draw_ = &PauseScene::NormalDraw;
		}
		return;
	}
}

void PauseScene::IntervalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	int center_y = wsize.h / 2;//画面の真ん中のY座標
	float rate = static_cast<float>(frame_) / 
					static_cast<float>(appear_interval);//表示割合

	int frame_height=(wsize.h - frame_margin) - center_y;//下端から中心を引いてる
	frame_height *= rate;

	//黒くて薄いセロファンを貼る
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(frame_margin, //左
			center_y - frame_height, //上
			wsize.w - frame_margin, //右
			center_y + frame_height, //下
			0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	//外枠
	DrawBox(frame_margin, //左
			center_y - frame_height, //上
			wsize.w - frame_margin, //右
			center_y + frame_height, //下
			0xaaaaff, false, 3);
}
void PauseScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	//黒くて薄いセロファンを貼る
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(frame_margin, frame_margin, //左上(10,10)
		wsize.w - frame_margin, wsize.h - frame_margin,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//外枠
	DrawBox(frame_margin, frame_margin, //左上(10,10)
		wsize.w - frame_margin, wsize.h - frame_margin,
		0xaaaaff, false, 3);

	//ポーズシーンの文字列表示
	DrawString(280, frame_margin+20, L"Pause Scene", 0xffffff);
	DrawMenu();
}

void PauseScene::DrawMenu()
{

	int menuStartX = frame_margin + menu_left_margin;
	
	int indicatorX = menuStartX-30;
	int menuY = frame_margin + menu_top_margin;
	for (int idx = 0; idx < menuList_.size(); ++idx) {
		int offsetX = 0;
		uint32_t col = 0xffffff;
		if (idx == selectIndex_) {
			DrawString(indicatorX, menuY, L"⇒", 0xffaaaa);
			offsetX = 10;
			col = GetColor(128, 255, 192);
		}
		DrawFormatString(menuStartX+offsetX, menuY,
						col,
						L"%s", menuList_[idx].c_str());

		menuY += menu_row_height;
	}
}

void PauseScene::YesNoDialogDraw()
{
	//NormalDrawを書いてるのは、通常のメニューの上に
	//YesNoDialogを置きたいので、メニューの表示をするため
	NormalDraw();

	constexpr int yes_no_dialog_height = 100;
	constexpr int yes_no_dialog_width = 300;
	const int centerX = Application::GetInstance().GetWindowSize().w / 2;
	const int centerY = Application::GetInstance().GetWindowSize().h / 2;

	//YES/NOの枠表示
	const int dialog_left = centerX - yes_no_dialog_width/2;
	DrawBox(
		dialog_left, 
		centerY - yes_no_dialog_height/2,
		centerX + yes_no_dialog_width/2,
		centerY + yes_no_dialog_height/2,
		0x8888bb, true);
	DrawBox(dialog_left, 
		centerY - yes_no_dialog_height/2,
		centerX + yes_no_dialog_width/2,
		centerY + yes_no_dialog_height/2,
		0xffffff, false,3);

	int y = centerY - 10;//画面中心から文字サイズの半分引く
	int x = dialog_left + 90;//はい、いいえが真ん中に来るよう調整
	std::array<std::wstring, 2> answers = {L"はい",L"いいえ"};

	//ダイアログタイトルを表示
	DrawFormatString(centerX - 50, centerY - yes_no_dialog_height / 2+10,
					0xffffff, L"%s", menuList_[selectIndex_].c_str());

	//はい、いいえを表示
	for (int idx = 0; idx < 2; ++idx) {
		uint32_t col = 0xffffff;
		if (yesNoIndex_ == idx) {
			DrawString(x - 20, y, L"⇒", 0xaaffaa);
			col = GetColor(255, 192, 192);
		}
		DrawFormatString(x, y, col, L"%s", answers[idx].c_str());
		x += 100;
	}

}

PauseScene::PauseScene(SceneController& controller):Scene(controller),
update_(&PauseScene::AppearUpdate),
draw_(&PauseScene::IntervalDraw)
{
	menuList_ = {
		L"ゲームに戻る",
		L"キーコンフィグ",
		L"タイトルに戻る",
		L"ゲームを終了する"
	};
	
	//メニューで選ばれる文字列と実行される内容のペアを定義
	execTable_[L"ゲームに戻る"] = [this](Input&) {
			update_ = &PauseScene::DisappearUpdate;
			draw_ = &PauseScene::IntervalDraw;
			frame_ = appear_interval;
		};
	execTable_[L"キーコンフィグ"] = [this](Input& input) {
		controller_.PushScene(std::make_shared<KeyConfigScene>(controller_,input));
		};
	execTable_[L"タイトルに戻る"] = [this](Input&) {
		update_ = &PauseScene::YesNoDialogUpdate;
		draw_ = &PauseScene::YesNoDialogDraw;
		yesRequestFunction_ = [this]() {
			controller_.ResetScene(std::make_shared<TitleScene>(controller_));
			};
		};
	execTable_[L"ゲームを終了する"] = [this](Input&) {
		update_ = &PauseScene::YesNoDialogUpdate;
		draw_ = &PauseScene::YesNoDialogDraw;
		yesRequestFunction_ = []() {
				Application::GetInstance().RequestExit();//ゲームを終了する
			};
		};

}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}
