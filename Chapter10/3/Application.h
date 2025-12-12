#pragma once

#include"Geometry.h"

/// <summary>
/// アプリケーション全体を管理する
/// シングルトンクラス
/// </summary>
class Application
{
private:
	Size windowSize_;
	//privateにコンストラクタ、コピーコンストラクタ、代入を
	//置くことで、２つ以上インスタンスを作れなくする
	Application();//newも変数宣言もできなくなる
	Application(const Application& app)=delete;//コピーコンストラクタも無効にする
	void operator=(const Application& app)=delete;//代入も一応無効にする

	bool requestedExit_ = false;//ゲームの終了がリクエストされた

public:
	~Application();
	//ただひとつのApplicationオブジェクトにアクセスする関数をstaticで用意
	//staticでないと、インスタンスがないと呼び出せないため、これはstaticで
	//オブジェクトがなくても呼び出せる
	
	/// <summary>
	/// Applicationのシングルトンインスタンスを取得する
	/// </summary>
	/// <returns>Applicationのインスタンス</returns>
	/// <note>Applicationの実体はこの関数内部で生成され、ずっととどまり続ける</note>
	static Application& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功:true / 初期化失敗:false</returns>
	bool Init();
	/// <summary>
	/// アプリケーションをスタートする
	/// ゲームループを内包する
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーション終了処理
	/// </summary>
	void Terminate();

	/// <summary>
	/// ウィンドウサイズを返す
	/// </summary>
	/// <returns>ウィンドウサイズ</returns>
	const Size& GetWindowSize()const;

	/// <summary>
	/// アプリケーションの終了する命令を飛ばす
	/// </summary>
	void RequestExit();

};

