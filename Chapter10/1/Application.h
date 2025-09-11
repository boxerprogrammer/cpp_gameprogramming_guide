#pragma once

#include "Geometry.h"

constexpr int default_window_width = 640;
constexpr int default_window_height = 480;

/// <summary>
/// アプリケーションの起動と終了を
/// コントロールするシングルトンクラス
/// </summary>
class Application
{
private:
	Application();//コンストラクタをprivateに
	//コピー＆代入の禁止
	Application(const Application& app) = delete;
	void operator=(const Application& app) = delete;

	Size windowSize_ = {default_window_width,default_window_height};

public:
	~Application();//デストラクタ
	/// <summary>
	/// Applicationクラスのインスタンスを返す
	/// </summary>
	/// <returns></returns>
	static Application& GetInstance();

	const Size& GetWindowSize()const;

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功 / false:初期化失敗</returns>
	bool Init(int w=default_window_width, int h=default_window_height);

	/// <summary>
	/// ゲームを起動する(メインループを実行)
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	void Terminate();
};

