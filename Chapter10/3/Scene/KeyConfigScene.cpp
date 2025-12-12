#include "KeyConfigScene.h"
#include<DxLib.h>
#include<array>
#include<cassert>
#include"../Input.h"
#include"SceneController.h"
#include"../Application.h"
#include"../StringFunctions.h"

constexpr int appear_interval = 10;//枠が出現するまでのフレーム数
constexpr int menu_row_height = 50;//メニューの行の高さ
constexpr int menu_left_margin = 80;//メニュー枠からの左余白
constexpr int menu_top_margin = 120;//メニュー枠からの上余白

constexpr int cursor_offset = -20;//カーソル左オフセット
constexpr int row_height = 30;//行の高さ
constexpr int event_name_width = 50;//イベント名の幅
constexpr int event_and_menu_margin = 50;//イベントリストとメニューリストの間の高さ
constexpr int selected_row_little_step = 10;//選択行をちょっとずらす幅
constexpr int input_data_width = 250;//実データの幅

KeyConfigScene::KeyConfigScene(SceneController& ct,Input& input):
	Scene(ct),
	input_(input),
	update_(&KeyConfigScene::AppearUpdate),
	draw_(&KeyConfigScene::IntervalDraw),
	frame_(0)
{
	input.tempInputTable_ = input.inputTable_;
	keyNameTable_[KEY_INPUT_LSHIFT] = L"左Shiftキー";
	keyNameTable_[KEY_INPUT_RSHIFT] = L"右Shiftキー";
	keyNameTable_[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
	keyNameTable_[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
	keyNameTable_[KEY_INPUT_ESCAPE] = L"Escキー";
	keyNameTable_[KEY_INPUT_SPACE] = L"スペースキー";
	keyNameTable_[KEY_INPUT_BACK] = L"BackSpaceキー";
	keyNameTable_[KEY_INPUT_TAB] = L"Tabキー";
	keyNameTable_[KEY_INPUT_RETURN] = L"Enterキー";
	keyNameTable_[KEY_INPUT_A] = L"Ａキー";
	keyNameTable_[KEY_INPUT_B] = L"Ｂキー";
	keyNameTable_[KEY_INPUT_C] = L"Ｃキー";
	keyNameTable_[KEY_INPUT_D] = L"Ｄキー";
	keyNameTable_[KEY_INPUT_E] = L"Ｅキー";
	keyNameTable_[KEY_INPUT_F] = L"Ｆキー";
	keyNameTable_[KEY_INPUT_G] = L"Ｇキー";
	keyNameTable_[KEY_INPUT_H] = L"Ｈキー";
	keyNameTable_[KEY_INPUT_I] = L"Ｉキー";
	keyNameTable_[KEY_INPUT_J] = L"Ｊキー";
	keyNameTable_[KEY_INPUT_K] = L"Ｋキー";
	keyNameTable_[KEY_INPUT_L] = L"Ｌキー";
	keyNameTable_[KEY_INPUT_M] = L"Ｍキー";
	keyNameTable_[KEY_INPUT_N] = L"Ｎキー";
	keyNameTable_[KEY_INPUT_O] = L"Ｏキー";
	keyNameTable_[KEY_INPUT_P] = L"Ｐキー";
	keyNameTable_[KEY_INPUT_Q] = L"Ｑキー";
	keyNameTable_[KEY_INPUT_R] = L"Ｒキー";
	keyNameTable_[KEY_INPUT_S] = L"Ｓキー";
	keyNameTable_[KEY_INPUT_T] = L"Ｔキー";
	keyNameTable_[KEY_INPUT_U] = L"Ｕキー";
	keyNameTable_[KEY_INPUT_V] = L"Ｖキー";
	keyNameTable_[KEY_INPUT_W] = L"Ｗキー";
	keyNameTable_[KEY_INPUT_X] = L"Ｘキー";
	keyNameTable_[KEY_INPUT_Y] = L"Ｙキー";
	keyNameTable_[KEY_INPUT_Z] = L"Ｚキー";
	
	padNameTable_[PAD_INPUT_A] = L"Ａボタン";
	padNameTable_[PAD_INPUT_B] = L"Ｂボタン";
	padNameTable_[PAD_INPUT_C] = L"Ｘボタン";
	padNameTable_[PAD_INPUT_X] = L"Ｙボタン";
	padNameTable_[PAD_INPUT_Y] = L"Ｌボタン";
	padNameTable_[PAD_INPUT_Z] = L"Ｒボタン";
	padNameTable_[PAD_INPUT_L] = L"SELECTボタン";
	padNameTable_[PAD_INPUT_R] = L"STARTボタン";
	padNameTable_[PAD_INPUT_START] = L"左レバー押込";
	padNameTable_[PAD_INPUT_M] = L"右レバー押込";

	//表示するメニューのリストを定義
	menuList_ = {	
					L"Save&Exit KeyConfig",//セーブしてキーコンフィグ終了
					L"Cancel&Exit KeyConfig",//キャンセルしてキーコンフィグ終了
					L"Reset Input Data" //キーの状態を「工場出荷時状態」に戻す
				};
	constexpr int save_exit = 0;
	constexpr int cancel_exit = 1;
	constexpr int reset_input_data = 2;
	//それぞれの選択に合わせた実行内容の定義
	menuFunctions_[menuList_[save_exit]] = [this]() {
		//閉じる前に実際のinput_.inputTable_へ変更内容を反映
		input_.inputTable_ = input_.tempInputTable_;//Save部分
		input_.Save();
		CloseSelf();
		};
	menuFunctions_[menuList_[cancel_exit]] = [this]() {
		//自分を閉じる
		CloseSelf();
		};
	menuFunctions_[menuList_[reset_input_data]] = [this]() {
		input_.InitializeInputTable();
		input_.tempInputTable_ = input_.inputTable_;
		};


}
void KeyConfigScene::CloseSelf()
{
	//自分を閉じる
	update_ = &KeyConfigScene::DisappearUpdate;
	draw_ = &KeyConfigScene::IntervalDraw;
}
void KeyConfigScene::AppearUpdate(Input& ) {
	if (++frame_ >= appear_interval) {
		update_ = &KeyConfigScene::NormalUpdate;
		draw_ = &KeyConfigScene::NormalDraw;
		return;
	}
}
void KeyConfigScene::NormalUpdate(Input& input) {
	//変更できるイベントの数＋メニューの数
	const int menuRowSize = input_.editableEventNames_.size() + menuList_.size();

	//OKを押されたら、エディットモードに移行するか、メニューを実行する
	if (input.IsTriggered("ok")) {
		//これは「最後の行」なので、Exitである
		//なので、ここでokが押されたらもう閉じる状態に移行する
		if (selectIndex_ < input_.editableEventNames_.size()) {//イベント名に止まってる
			//もし、イベント名のところでOKが押されていたら
			//エディットモードに切り替える
			update_ = &KeyConfigScene::EditingUpdate;
			draw_ = &KeyConfigScene::EditingDraw;
			isFirstEditFrame_ = true;//EditingUpdateの最初のフレームだけは無視する(生入力だけ更新)
		}else{//メニュー部分を選択中
			//selectIndex_は、イベントリストとメニューリストの合成なので
			//メニュー部分を選択されている場合は、イベントリストサイズ分引くことで
			//メニューのインデックスになります
			auto menuIndex = selectIndex_ - input_.editableEventNames_.size();
			auto menuName = menuList_[menuIndex];
			if (menuFunctions_.contains(menuName)) {
				menuFunctions_[menuName]();
			}
			else {
				assert(0);
			}
		}
		return;
	}
	
	if (input.IsTriggered("up")) {
		selectIndex_ = (selectIndex_ + menuRowSize - 1) % menuRowSize;
	}
	if (input.IsTriggered("down")) {
		selectIndex_ = (selectIndex_ + 1) % menuRowSize;
	}
}
void KeyConfigScene::DisappearUpdate(Input&) {
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}
void KeyConfigScene::EditingUpdate(Input& input )
{
	//ここではメニューの移動処理がないため「現在の選択行」は固定されています
	
	//OKボタンを押せばまた非エディットモードに戻る
	if (input.IsTriggered("ok")) {
		update_ = &KeyConfigScene::NormalUpdate;//非エディットモード
		draw_ = &KeyConfigScene::NormalDraw;
		isFirstEditFrame_ = false;
		return;
	}
	//生入力処理
	//トリガー状態にするため今の入力→直前入力
	lastKeyState_ = currentKeyState_;
	lastPadState_ = currentPadState_;
	//生入力を取得します
	GetHitKeyStateAll(currentKeyState_.data());//キーボード情報
	currentPadState_=GetJoypadInputState(DX_INPUT_PAD1);//パッド情報

	//NormalUpdateから移行した最初のフレームだけはisFirstEditFrameがtrueになっているため
	//ここで、なにもしないで抜けます
	if (isFirstEditFrame_) {
		//２回目以降はちゃんとエディットできるようにしておく
		isFirstEditFrame_ = false;//これをしないと永遠にキーコンフィグできない注意
		return;
	}
	//エディットチェック開始
	//現在選択中のイベント名を取得
	auto eventName = input_.editableEventNames_[selectIndex_];
	//編集中の行のイベント情報(一時的な)を取得しておきます
	auto& editingRowData = input_.tempInputTable_[eventName];
	//キーボードの入力をチェックする
	
	//このループはGetHitKeyStateで得られた配列のループ
	for (int keyCode = 0; keyCode < currentKeyState_.size();++keyCode) {
		//あらかじめ想定された入力以外だった場合は受け付けない
		if (!keyNameTable_.contains(keyCode))continue;
		//
		if (currentKeyState_[keyCode] && !lastKeyState_[keyCode]) {//最初にtrueのものを見つけたらそれをキーコードとする
			//現在選択中のイベントの内容を更新する
			//キーボード＆パッドループ
			for (auto& inputRow : editingRowData) {
				if (inputRow.type == PeripheralType::keyboard) {
					inputRow.id = keyCode;//イベントと生入力の対応関係を更新(キーボード)
				}
			}
			break;
		}
	}
	//パッド入力をチェック
	//パッド入力はひとつの整数型のビットでどれが押されてるかを表現しているため
	//インデックスのように++ではなく、ビットを左に、左にずらしながら判定する
	uint32_t bit = 0b1;
	//32bitぶんチェックする
	for (int i = 0; i < 32; ++i) {
		//もし、そのビットが登録されていなければ
		//次のビットに進める
		if (!padNameTable_.contains(bit)) {
			bit <<= 1;//continueの際でもbitは進めないと止まってしまう
			continue;
		}
		//もし、そのビットが登録されているビットならば、そのビットで
		//入力テーブルを更新する
		if ((currentPadState_ & bit) && !(lastPadState_&bit)) {
			for (auto& inputRow : editingRowData) {
				if (inputRow.type == PeripheralType::pad1) {
					inputRow.id = bit;
				}
			}
			break;
		}
		//<<=nというのは、左辺値の現在の数値(2進数)を左にnだけずらす
		bit <<= 1;//ヒットしなかったらビットを先に進める
	}

}
void KeyConfigScene::NormalDraw() {
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto centerX = wsize.w / 2;
	const auto frameSize = MaxFrameSize();
	const auto frameW = frameSize.w / 2;
	
	int y = DrawInputTableList();//イベントリスト(入力テーブルリスト)描画

	y += event_and_menu_margin;//下のメニューとの間に余白を作る

	//メニューの表示
	//メニューリストぶんループ
	for (const auto& name : menuList_) {
		//基準になるX座標を決めておく
		auto x = centerX - frameW + menu_left_margin + 150;//画面真ん中あたり
		//もし選択行がメニュー側だったら…(イベント名のサイズ以上であればメニュー側)
		if(selectIndex_>=input_.editableEventNames_.size()){
			//メニュー側選択中なので、インデックスをメニュー側インデックスに変換
			auto menuIndex = selectIndex_ - input_.editableEventNames_.size();
			if (menuList_[menuIndex] == name) {
				DrawString(x + cursor_offset, y, L"⇒", 0xff0000);
				x += selected_row_little_step;//選択中の行はちょっと右にずらす
			}
		}
		//メニュー名の表示
		DrawFormatString(x, y,  0xffffff, L"%s",name.c_str());
		y += row_height;
	}
}
void KeyConfigScene::EditingDraw()
{
	DrawInputTableList(0xaa00ff);
}
void KeyConfigScene::IntervalDraw() {
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);
	const auto& wsize = Application::GetInstance().GetWindowSize();
	auto frameSize = MaxFrameSize();
	DrawFrame(wsize.w / 2, wsize.h / 2,//画面中心
			(frameSize.w / 2) * rate, frameSize.h / 2);//幅・高さ
}
int KeyConfigScene::DrawInputTableList(uint32_t selectColor)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto frameSize = MaxFrameSize();
	const auto centerX = wsize.w / 2;
	const auto centerY = wsize.h / 2;
	const auto frameW = frameSize.w / 2;
	const auto frameH = frameSize.h / 2;
	//枠表示
	DrawFrame(centerX, centerY, frameW, frameH);

	//名前のテーブルを作っておく
	std::map<PeripheralType, std::wstring> periNameTable;
	periNameTable[PeripheralType::keyboard] = L"キーボード";
	periNameTable[PeripheralType::pad1] = L"パッド";
	constexpr int title_y_from_frame = 10;
	constexpr int list_top_from_title = 100;
	constexpr int title_x_from_center = -50;

	//キーコンフィグのテキスト
	int y = centerY - frameH;//枠の上端
	//ここ以降でy+=?と書いている部分は、少しずつyを下に下げていきつつ
	//文字列を表示するため
	y += title_y_from_frame;//枠の上端から10ピクセル移動
	//キーコンフィグタイトル
	DrawString(centerX + title_x_from_center, y, L"Key Config", 0xffffff);
	y += list_top_from_title;
	//イベント名と、実際に割り当てられているボタンを表示します
	for (const auto& name : input_.editableEventNames_) {
		//編集中状態で、もし現在編集中の行を選択していれば
		//選択中の行の色を変更し、もうちょっとだけ右にずらす
		int x = centerX - frameW + menu_left_margin;
		uint32_t col = 0xffffff;//非選択の行は白
		if (selectIndex_ < input_.editableEventNames_.size()) {
			if (input_.editableEventNames_[selectIndex_] == name) {
				x += selected_row_little_step;//ちょっと右にずらす
				DrawString(x + cursor_offset, y, L"⇒", 0xff0000);
				col = selectColor;//行の色を変更
				x += selected_row_little_step;
			}
		}
		//イベント名を表示
		auto wname = StringFunctions::WStringFromString(name);
		DrawFormatString(x, y, col, L"%s", wname.c_str());
		//右にちょっとずらして、実際の入力コードを表示
		x += event_name_width;

		//ここでの表示は一時テーブルの内容を表示するため
		//input_.inputTableではなくinput_.tempInputTable_にするのを
		//わすれないでください
		for (const auto& inputState : input_.tempInputTable_[name]) {
			DrawFormatString(x, y, col, L"%s : %s",
				periNameTable[inputState.type].c_str(),
				inputState.type == PeripheralType::keyboard ?
				keyNameTable_[inputState.id].c_str() :
				padNameTable_[inputState.id].c_str());
			x += input_data_width;
		}
		y += row_height;
	}
	return y;
}
void KeyConfigScene::DrawFrame(int centerX,int centerY, int w, int h)
{
	//キーコンフィグの背景
	DrawBox(centerX - w,
		centerY - h,
		centerX + w,
		centerY + h,
		0x44aa44, true);

	//キーコンフィグの外枠
	DrawBox(centerX - w,
		centerY - h,
		centerX + w,
		centerY + h,
		0xffffff, false, 3);
}
Size KeyConfigScene::MaxFrameSize()const
{
	constexpr int frame_margin = 20;//画面端からの余白
	const auto& wsize = Application::GetInstance().GetWindowSize();
	return { wsize.w - frame_margin,wsize.h - frame_margin };
}
void KeyConfigScene::Update(Input& input)
{
	(this->*update_)(input);
}

void KeyConfigScene::Draw()
{
	(this->*draw_)();
}
