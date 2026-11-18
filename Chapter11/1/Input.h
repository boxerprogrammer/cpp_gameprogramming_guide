#pragma once
#include<array>
#include<string>
#include<map>
#include<vector>


/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType {
	keybd,
	pad,
	mouse
};

//実入力情報
struct InputInfo {
	PeripheralType type;//入力機器種別
	int id;//入力ID
};

class KeyconfigScene;
/// <summary>
/// 入力をまとめるクラス
/// </summary>
class Input
{
	friend KeyconfigScene;
private:
	std::vector<std::string> editableList_;//キーコンフィグで編集可能なコードリスト
	//
	using InputTable_t = std::map < std::string, std::vector<InputInfo>>;
	InputTable_t inputTable_;//入力テーブル(入力コード(文字列)と実入力の対応表)←押したか押してないかは気にしない
	InputTable_t tmpInputTable_;//一時入力テーブル(キーコンフィグでのみ使用)

	//コードに対応する入力がされているかを記憶する(mapの文字列とbool変数で押したか押してないかを記録)
	using InputState_t = std::map<std::string, bool>;//キー文字列と、押したか押してないかbool
	InputState_t currentInput_;//現在の押されてる状況
	InputState_t lastInput_;//最後に押された状況
	/// <summary>
	/// 入力テーブルをリセットする
	/// </summary>
	void ResetInputSetting();
public:
	Input();
	
	/// <summary>
	/// 今ボタンを押している状態か？
	/// </summary>
	/// <returns>true:今押してる / false:押してない</returns>
	bool IsPressed(const std::string& code)const;

	/// <summary>
	/// 今、押した瞬間か？(押しっぱなしでは反応しない)
	/// </summary>
	/// <returns>true:押した瞬間 / false:押してないor押しっぱなし</returns>
	bool IsTriggered(const std::string& code)const;

	/// <summary>
	/// 入力状態を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 現在のキーコンフィグ情報をセーブする
	/// </summary>
	void Save();

	/// <summary>
	/// セーブ済みのキーコンフィグ情報をロードする
	/// </summary>
	void Load();

};

