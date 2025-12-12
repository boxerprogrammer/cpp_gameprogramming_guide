#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<map>
#include<string>
#include<array>
#include<vector>//キー以外のメニュー用
#include<functional>//メニュー実行用
/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyConfigScene : public Scene
{
private:
	int selectIndex_=0;//現在選択中のインデックス

	std::map<uint32_t, std::wstring> padNameTable_;//パッドのボタン名テーブル
	std::map<int, std::wstring> keyNameTable_;//キーボードのキー名テーブル

	int currentPadState_ = 0;//パッドの入力が行われていたか
	std::array<char, 256> currentKeyState_ = {};//キーボードの入力が行われていたか
	int lastPadState_ = 0;//直前でパッドの入力が行われていたか
	std::array<char, 256> lastKeyState_ = {};//直前でキーボードの入力が行われていたか

	std::vector<std::wstring> menuList_;//メニューの文字列リスト
	std::map < std::wstring, std::function<void(void)>> menuFunctions_;//メニュー文字列と実行内容の対応

	bool isFirstEditFrame_ = false;

	//drawなどで使用するInputクラスオブジェクトへの参照
	Input& input_;
	//ウィンドウを開く時のフレームカウント
	int frame_;

	using UpdateFunc_t = void(KeyConfigScene::*)(Input&);
	UpdateFunc_t update_;
	using DrawFunc_t = void(KeyConfigScene::*)();
	DrawFunc_t draw_;

	//Update系状態関数
	void AppearUpdate(Input&);
	void NormalUpdate(Input&);
	void DisappearUpdate(Input&);

	//現在、キーの内容を編集中
	void EditingUpdate(Input&);

	//Draw系状態関数
	void NormalDraw();

	/// <summary
	/// 編集中描画
	/// </summary>
	void EditingDraw();

	/// <summary>
	/// フェード中の描画
	/// </summary>
	void IntervalDraw();

	/// <summary>
	/// イベント名を描画する
	/// </summary>
	/// <return>リスト描画後のY座標を返す</return>
	int DrawInputTableList(uint32_t selectColor=0xffffff);

	/// <summary>
	/// キーコンフィグの枠を表示
	/// </summary>
	/// <param name="w">枠の幅</param>
	/// <param name="h">枠の高さ</param>
	void DrawFrame(int centerX, int centerY,int w, int h);
	/// <summary>
	/// 最大枠サイズを返す
	/// </summary>
	/// <returns>最大枠サイズ</returns>
	Size MaxFrameSize()const;
public:
	/// <summary>
	/// KeyConfigSceneコンストラクタ
	/// </summary>
	/// <param name="ctrl">シーンコントローラ</param>
	/// <param name="input">Inputオブジェクト</param>
	KeyConfigScene(SceneController& ctrl,Input& input);
	void CloseSelf();
	void Update(Input& input)override;
	void Draw()override;
};

