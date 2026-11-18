#pragma once
#include"Scene.h"
#include<map>
#include<string>
#include<array>
#include<functional>
class Input;
struct InputInfo;
/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyconfigScene : public Scene
{
private:
	std::vector<std::map<int, std::wstring>> nameTables_;

	std::vector<std::wstring> menuItems_;//コンフィグ以外のメニュー
	std::map < std::wstring, std::function<void(Input&)>> menuFunctions_;//選んだメニューに対する実行内容

	
	//直前の生入力
	std::array<char, 256> lastKeyState_;
	int lastPad_;
	int lastMouse_;

	//生入力の更新
	void UpdateLastInput();

	int selectedIndex_ = 0;//選択中の行インデックス

	int frame_ = 0;
	Input& input_;
	using UpdateFunc_t = void(KeyconfigScene::*)(Input&);
	using DrawFunc_t = void(KeyconfigScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void AppearUpdate(Input&);
	void NormalUpdate(Input& input);
	void EditUpdate(Input& input);
	void UpdateRawMouseInput(int mouseInput);
	void UpdateRawPadInput(int padInput, std::vector<InputInfo>& inputInfoes);
	void UpdateRawKeyboardInput(std::array<char, 256Ui64>& keyState, std::vector<InputInfo>& inputInfoes);
	void DisappearUpdate(Input&);

	//Draw系の関数(draw_が選択できる関数)
	void NormalDraw();//通常時の描画
	void EditDraw();//編集時の描画
	void TransitionEffectDraw();//切り替え演出描画

	void DrawKeyconfigBoard();

	/// <summary>
	/// コンフィグ可能な入力リストの表示を行う
	/// </summary>
	/// <param name="selectedTextCol">選択中行の色</param>
	int DrawInputList(unsigned int selectedTextCol=0xeecccc,int selectedOffset=5);
	
	/// <summary>
	/// 影付きの文字列を描画する
	/// </summary>
	/// <param name="str">文字列</param>
	void DrawStringWithShadow(int x, int y, uint32_t textCol,const std::wstring& str);

	void CloseWindow();
public:
	KeyconfigScene(SceneController& ctrl,Input& input);
	void InitMouseInputNameTable();
	void InitPadInputNameTable();
	void InitKeyboardNameTable();
	void Update(Input& input)override;
	void Draw()override;
};

