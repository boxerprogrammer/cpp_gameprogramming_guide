#pragma once
#include<string>
#include<map>
#include<memory>
enum class AssetType {
	continuous,//常駐アセット(アプリ終了時まで解放しない)
	temporary,//一時的なもの
};
enum class FileType {
	graphic,//画像
	sound//音声
};
class FileLoader
{
	//このクラス自体はFileLoaderが知っていればそれでいい
	//使用者側はハンドルさえ持ってればOK
	class Asset {
		friend FileLoader;
	private:
		AssetType type=AssetType::continuous;
		FileType fileType=FileType::graphic;
		int handle=-1;
	public:
		Asset(){}
		~Asset();
	};
	std::map<std::wstring, std::shared_ptr<Asset>> assets_;//ロード済みのアセットを名前で管理する
public:
	/// <summary>
	/// 画像をロード
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="type">常駐か一時的か(一時的ならtemporaryを選択)</param>
	/// <returns>ハンドル</returns>
	int LoadGraph(const std::wstring& path,AssetType type=AssetType::continuous);

	/// <summary>
	/// サウンドをロード
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="type">常駐か一時的か(一時的ならtemporaryを選択)</param>
	/// <returns>ハンドル</returns>
	int LoadSound(const std::wstring& path,AssetType type = AssetType::continuous);
	
	/// <summary>
	/// 一時アセット(temporary)のみを削除する
	/// </summary>
	void DeleteTemporalAssets();

};

