#pragma once
#include<memory>

#include"Geometry.h"

class FileLoader;//プロトタイプ宣言

//アプリケーションクラス(Singleton)
//ゲーム中で１つしかないクラス
//このクラスで、ゲームの起動、メインループ、終了のコントロールをしたい
class Application
{
private:
	std::shared_ptr<FileLoader> loader_;//ファイルローダ
	int bgmVolume_ = 100;
	int seVolume_ = 100;
	bool requestExit_=false;//アプリの終了がリクエストされたか
	Size wsize_;
	//コンストラクタをprivateにして外側で実体を作らせないように
	Application();
	//コピー禁止
	Application(const Application&)=delete;
public:

	//唯一アプリケーションクラスの実体にアクセスする関数
	static Application& GetInstance();

	~Application() {}

	//ゲームの起動(初期化)
	bool Init();

	//ゲームのメインループを走らせる
	void Run();

	//ゲームの終了処理
	void Terminate();
	
	/// <summary>
	/// ウィンドウのサイズを取得
	/// </summary>
	/// <returns>ウィンドウのサイズ</returns>
	const Size& GetWindowSize()const;

	/// <summary>
	/// アプリケーションの終了をリクエストする
	/// </summary>
	void RequestExit();

	int GetBGMVolume()const;
	void SetBGMVolume(int volume);

	int GetSEVolume()const;
	void SetSEVolume(int volume);

	std::shared_ptr<FileLoader> GetLoader();

};

