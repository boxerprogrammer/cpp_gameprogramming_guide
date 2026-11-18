#include<DxLib.h>
#include "KeyconfigScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../Application.h"
#include"../StringUtility.h"
constexpr int margin_size = 60;
constexpr int keyconfig_left = 200;
constexpr int keyconfig_top = 150;
constexpr int keyconfig_text_height = 30;
constexpr int keyconfig_text_width = 150;
constexpr int keyconfig_menu_left_offset = 180;
constexpr int keyconfig_menu_top_offset = 30;
constexpr int appear_interval = 15;
constexpr int arrow_offset = -30;

constexpr uint32_t shadow_text_color = 0x000000;
constexpr uint32_t white_text_color = 0xffffff;
constexpr uint32_t edit_text_color = 0xcc8800;
constexpr uint32_t selected_text_color = 0xffffaa;

constexpr uint32_t back_color = 0x8888ff;
constexpr uint32_t frame_color = 0xffffaa;

void KeyconfigScene::UpdateLastInput()
{
	GetHitKeyStateAll(lastKeyState_.data());
	lastPad_ = GetJoypadInputState(DX_INPUT_PAD1);
	lastMouse_ = GetMouseInput();
}

void KeyconfigScene::AppearUpdate(Input&)
{
	if (++frame_ >= appear_interval) {
		update_ = &KeyconfigScene::NormalUpdate;
		draw_ = &KeyconfigScene::NormalDraw;
		frame_ = 0;
	}
}

void KeyconfigScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		if (selectedIndex_ < input.editableList_.size()) {
			update_ = &KeyconfigScene::EditUpdate;
			draw_ = &KeyconfigScene::EditDraw;
			UpdateLastInput();
			return;
		}
		else {
			int menuIdx = selectedIndex_ - static_cast<int>(input.editableList_.size());
			menuFunctions_[menuItems_[menuIdx]](input);
			return;
		}
	}
	const int item_num = static_cast<int>(input.editableList_.size() + menuItems_.size());
	if (input.IsTriggered("up")) {
		selectedIndex_ = (selectedIndex_ + item_num - 1) % item_num;
	}
	if (input.IsTriggered("down")) {
		selectedIndex_ = (selectedIndex_ + 1) % item_num;
	}
}

void KeyconfigScene::EditUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &KeyconfigScene::NormalUpdate;
		draw_ = &KeyconfigScene::NormalDraw;
		return;
	}
	//生入力を取得
	std::array<char, 256> keyState;
	GetHitKeyStateAll(keyState.data());
	int padInput = GetJoypadInputState(DX_INPUT_PAD1);
	int mouseInput = GetMouseInput();

	//現在編集中の行の入力を変更
	auto code = input.editableList_[selectedIndex_];
	auto& inputInfoes = input.tmpInputTable_[code];

	UpdateRawKeyboardInput(keyState, inputInfoes);//キーボード生入力による割り当て更新
	UpdateRawPadInput(padInput, inputInfoes);//パッド生入力による割り当て更新
	UpdateRawMouseInput(mouseInput);//マウス生入力による割り当て更新


	lastKeyState_ = keyState;
	lastPad_ = padInput;
	lastMouse_ = mouseInput;
	
}

void KeyconfigScene::UpdateRawMouseInput(int mouseInput)
{
	//マウスの割り当て更新
	unsigned int mbit = 1;
	constexpr int mouse_max_bit_num = 8;
	const auto& mouseNames = nameTables_[static_cast<int>(PeripheralType::mouse)];
	for (int i = 0;i < mouse_max_bit_num;++i) {
		if ((mouseInput & mbit) && !(lastMouse_ & mbit)) {
			if (mouseNames.contains(mbit)) {
				continue;
			}
		}
		mbit <<= 1;
	}
}

void KeyconfigScene::UpdateRawPadInput(int padInput, std::vector<InputInfo>& inputInfoes)
{
	//パッドの割り当て更新
	unsigned int bit = 1;//一番右が立っているビット
	constexpr int pad_max_bit_num = 32;
	const auto& padNames = nameTables_[static_cast<int>(PeripheralType::pad)];
	for (int i = 0;i < pad_max_bit_num;++i) {//①
		if ((padInput & bit) && !(lastPad_ & bit)) {
			if (!padNames.contains(bit)) {
				continue;
			}
			bool hit = false;
			for (auto& inputInfo : inputInfoes) {//②
				if (inputInfo.type == PeripheralType::pad) {
					inputInfo.id = bit;
					hit = true;
					break;//②のfor文を抜けます
				}
			}
			if (hit) {
				break;//①のfor文を抜ける
			}
		}
		bit <<= 1;//１つずつ左にビットシフトします
	}
}

void KeyconfigScene::UpdateRawKeyboardInput(std::array<char, 256Ui64>& keyState, std::vector<InputInfo>& inputInfoes)
{
	//キーボードの割り当て更新(押されてなければただループが終わる)
	const auto& keyboardNames = nameTables_[static_cast<int>(PeripheralType::keybd)];
	for (int idx = 0;idx < keyState.size();++idx) {//idxは生キーインデックス
		if (keyState[idx] && !lastKeyState_[idx]) {
			//もしWASDや方向キーなどの編集不可の入力が来た場合は
			//つまりkeyboardNames_に登録してない入力が来た場合は無視します
			if (!keyboardNames.contains(idx)) {
				continue;
			}
			for (auto& info : inputInfoes) {
				if (info.type == PeripheralType::keybd) {
					info.id = idx;
					break;
				}
			}
			break;
		}
	}
}

void KeyconfigScene::DisappearUpdate(Input&)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}

void KeyconfigScene::NormalDraw()
{
	DrawKeyconfigBoard();

	//Input内部の入力リストを表示
	int y = DrawInputList();
	y += keyconfig_menu_top_offset;
	//キーコンフィグ以外のメニューリスト表示
	for(auto& item : menuItems_){
		unsigned int textCol = white_text_color;
		int x = margin_size + keyconfig_left+keyconfig_menu_left_offset;

		if (input_.editableList_.size() <= selectedIndex_) {
			int menuIndex = selectedIndex_ - static_cast<int>(input_.editableList_.size());
			if (item == menuItems_[menuIndex]) {
				DrawString(x + 2 + arrow_offset, y + 2, L"☞", shadow_text_color);//影
				DrawString(x + arrow_offset, y, L"☞", white_text_color);
				textCol = selected_text_color;
				x += 5;
			}
		}

		DrawFormatString(x+2, y+2, shadow_text_color, L"%s", item.c_str());
		DrawFormatString(x, y, textCol, L"%s", item.c_str());
		y += keyconfig_text_height;
	}
}

void KeyconfigScene::EditDraw()
{
	DrawKeyconfigBoard();

	//Input内部の入力リストを表示
	DrawInputList(0xcc8800,10);
}

void KeyconfigScene::DrawKeyconfigBoard()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//青っぽい面
	DrawBox(margin_size, margin_size,
		wsize.w - margin_size, wsize.h - margin_size, back_color, true);
	//枠
	DrawBox(margin_size, margin_size,
		wsize.w - margin_size, wsize.h - margin_size, frame_color, false, 3);

	DrawString(margin_size + 50, margin_size + 50, L"Keyconfig Scene", white_text_color);
}

int KeyconfigScene::DrawInputList(unsigned int selectedTextCol, int selectedOffset)
{
	int y = margin_size + keyconfig_top;
	for (auto& editableCode : input_.editableList_) {
		unsigned int textCol = white_text_color;
		int x = margin_size + keyconfig_left;
		if (selectedIndex_ < input_.editableList_.size()) {
			if (editableCode == input_.editableList_[selectedIndex_]) {
				DrawString(x + 2 + arrow_offset, y + 2, L"☞", shadow_text_color);//影
				DrawString(x + arrow_offset, y, L"☞", white_text_color);//本体
				textCol = selectedTextCol;
				x += selectedOffset;
			}
		}
		const auto& name = StringUtility::GetWstringFromString(editableCode);
		DrawFormatString(x + 2, y + 2, shadow_text_color, L"%s", name.c_str());//影
		DrawFormatString(x, y, textCol, L"%s", name.c_str());//本体
		auto& inputRow = input_.tmpInputTable_[editableCode];
		//実入力を表示
		for (auto& inputInfo : inputRow) {
			x += keyconfig_text_width;//隣の項目に移る
			auto& nameTable= nameTables_[static_cast<int>(inputInfo.type)];
			DrawStringWithShadow(x, y, textCol, nameTable[inputInfo.id]);
		}

		y += keyconfig_text_height;
	}
	return y;
}

void KeyconfigScene::TransitionEffectDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const auto center_x = wsize.w / 2;
	const auto frame_max_width = wsize.w / 2 - margin_size;
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);
	int currentWidth = static_cast<int>(frame_max_width * rate);

	//青っぽい面
	DrawBox(center_x - currentWidth, 
			margin_size,
			center_x + currentWidth,
			wsize.h - margin_size, 
		back_color, true);
	//枠
	DrawBox(center_x - currentWidth,
			margin_size,
			center_x + currentWidth,
			wsize.h - margin_size, 
			frame_color, false, 3);
}

KeyconfigScene::KeyconfigScene(SceneController& ctrl, Input& input):Scene(ctrl),input_(input),
update_(&KeyconfigScene::AppearUpdate),
draw_(&KeyconfigScene::TransitionEffectDraw),
lastKeyState_{},
lastPad_{},
lastMouse_{}
{
	nameTables_.resize(3);
	InitKeyboardNameTable();//キーボード名テーブルを初期化する
	InitPadInputNameTable();//パッド名テーブルを初期化する
	InitMouseInputNameTable();//マウス名テーブルを初期化する

	menuItems_ = { L"セーブして閉じる" ,L"戻る(キャンセル)",L"リセットして閉じる"};
	menuFunctions_[L"セーブして閉じる"] = [this](Input& input) {
		input.inputTable_ = input.tmpInputTable_;//編集済みのキーコンフィグを逆コピー
		input.Save();
		CloseWindow();
		return;
	};
	menuFunctions_[L"戻る(キャンセル)"] = [this](Input& ) {
		//戻るのときは、inputTable_には反映されません。
		CloseWindow();
		return;
	};
	menuFunctions_[L"リセットして閉じる"] = [this](Input& input) {
		input.ResetInputSetting();
		CloseWindow();
		return;
	};

	input_.tmpInputTable_ = input_.inputTable_;
}

void KeyconfigScene::InitMouseInputNameTable()
{
	auto& mouseTable = nameTables_[static_cast<int>(PeripheralType::mouse)];
	mouseTable[MOUSE_INPUT_LEFT] = L"マウス左";
	mouseTable[MOUSE_INPUT_RIGHT] = L"マウス右";
	mouseTable[MOUSE_INPUT_MIDDLE] = L"マウス中央";
}

void KeyconfigScene::InitPadInputNameTable()
{
	auto& padInputNames = nameTables_[static_cast<int>(PeripheralType::pad)];
	padInputNames[PAD_INPUT_A] = L"Ａボタン";
	padInputNames[PAD_INPUT_B] = L"Ｂボタン";
	padInputNames[PAD_INPUT_C] = L"Ｘボタン";
	padInputNames[PAD_INPUT_X] = L"Ｙボタン";
	padInputNames[PAD_INPUT_Y] = L"Ｌボタン";
	padInputNames[PAD_INPUT_Z] = L"Ｒボタン";
	padInputNames[PAD_INPUT_L] = L"STARTボタン";
	padInputNames[PAD_INPUT_R] = L"SELECTボタン";
	padInputNames[PAD_INPUT_START] = L"左押し込み";
	padInputNames[PAD_INPUT_M] = L"右押し込み";
}

void KeyconfigScene::InitKeyboardNameTable()
{
	auto& keyboardNames = nameTables_[static_cast<int>(PeripheralType::keybd)];
	keyboardNames[KEY_INPUT_BACK] = L"BackSpaceキー";
	keyboardNames[KEY_INPUT_TAB] = L"Tabキー";
	keyboardNames[KEY_INPUT_RETURN] = L"Enterキー";
	keyboardNames[KEY_INPUT_LSHIFT] = L"左Shiftキー";
	keyboardNames[KEY_INPUT_RSHIFT] = L"右Shiftキー";
	keyboardNames[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
	keyboardNames[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
	keyboardNames[KEY_INPUT_ESCAPE] = L"Escキー";
	keyboardNames[KEY_INPUT_SPACE] = L"スペースキー";
	keyboardNames[KEY_INPUT_END] = L"Endキー";
	keyboardNames[KEY_INPUT_HOME] = L"Homeキー";
	keyboardNames[KEY_INPUT_INSERT] = L"Insertキー";
	keyboardNames[KEY_INPUT_DELETE] = L"Deleteキー";
	keyboardNames[KEY_INPUT_F1] = L"Ｆ１キー";
	keyboardNames[KEY_INPUT_F2] = L"Ｆ２キー";
	keyboardNames[KEY_INPUT_F3] = L"Ｆ３キー";
	keyboardNames[KEY_INPUT_F4] = L"Ｆ４キー";
	keyboardNames[KEY_INPUT_F5] = L"Ｆ５キー";
	keyboardNames[KEY_INPUT_F6] = L"Ｆ６キー";
	keyboardNames[KEY_INPUT_F7] = L"Ｆ７キー";
	keyboardNames[KEY_INPUT_F8] = L"Ｆ８キー";
	keyboardNames[KEY_INPUT_F9] = L"Ｆ９キー";
	keyboardNames[KEY_INPUT_F10] = L"Ｆ１０キー";
	keyboardNames[KEY_INPUT_F11] = L"Ｆ１１キー";
	keyboardNames[KEY_INPUT_F12] = L"Ｆ１２キー";
	keyboardNames[KEY_INPUT_B] = L"Ｂキー";
	keyboardNames[KEY_INPUT_C] = L"Ｃキー";
	keyboardNames[KEY_INPUT_E] = L"Ｅキー";
	keyboardNames[KEY_INPUT_F] = L"Ｆキー";
	keyboardNames[KEY_INPUT_G] = L"Ｇキー";
	keyboardNames[KEY_INPUT_H] = L"Ｈキー";
	keyboardNames[KEY_INPUT_I] = L"Ｉキー";
	keyboardNames[KEY_INPUT_J] = L"Ｊキー";
	keyboardNames[KEY_INPUT_K] = L"Ｋキー";
	keyboardNames[KEY_INPUT_L] = L"Ｌキー";
	keyboardNames[KEY_INPUT_M] = L"Ｍキー";
	keyboardNames[KEY_INPUT_N] = L"Ｎキー";
	keyboardNames[KEY_INPUT_O] = L"Ｏキー";
	keyboardNames[KEY_INPUT_P] = L"Ｐキー";
	keyboardNames[KEY_INPUT_Q] = L"Ｑキー";
	keyboardNames[KEY_INPUT_R] = L"Ｒキー";
	keyboardNames[KEY_INPUT_T] = L"Ｔキー";
	keyboardNames[KEY_INPUT_U] = L"Ｕキー";
	keyboardNames[KEY_INPUT_V] = L"Ｖキー";
	keyboardNames[KEY_INPUT_X] = L"Ｘキー";
	keyboardNames[KEY_INPUT_Y] = L"Ｙキー";
	keyboardNames[KEY_INPUT_Z] = L"Ｚキー";
	keyboardNames[KEY_INPUT_0] = L"０キー";
	keyboardNames[KEY_INPUT_1] = L"１キー";
	keyboardNames[KEY_INPUT_2] = L"２キー";
	keyboardNames[KEY_INPUT_3] = L"３キー";
	keyboardNames[KEY_INPUT_4] = L"４キー";
	keyboardNames[KEY_INPUT_5] = L"５キー";
	keyboardNames[KEY_INPUT_6] = L"６キー";
	keyboardNames[KEY_INPUT_7] = L"７キー";
	keyboardNames[KEY_INPUT_8] = L"８キー";
	keyboardNames[KEY_INPUT_9] = L"９キー";
}

void KeyconfigScene::DrawStringWithShadow(int x,int y ,uint32_t textCol , const std::wstring& text)
{
	DrawFormatString(x + 2, y + 2, shadow_text_color, L"%s", text.c_str());//影
	DrawFormatString(x, y, textCol, L"%s", text.c_str());//本体
}

void KeyconfigScene::CloseWindow()
{
	update_ = &KeyconfigScene::DisappearUpdate;
	draw_ = &KeyconfigScene::TransitionEffectDraw;
	frame_ = appear_interval;
}

void KeyconfigScene::Update(Input& input)
{
	(this->*update_)(input);
}

void KeyconfigScene::Draw()
{
	(this->*draw_)();
}
