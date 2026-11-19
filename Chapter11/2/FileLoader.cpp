#include "FileLoader.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>

namespace {
//リソースディレクトリのパス
//この変数はFileLoader.cpp内でのみ有効
wchar_t resource_dir[] = L"../resource/";
}
FileLoader::Asset::~Asset()
{
	//アセットが削除されるときにハンドルも自動削除しておく
	switch (fileType) {
	case FileType::graphic:
		DeleteGraph(handle);
		break;
	case FileType::sound:
		DeleteSoundMem(handle);
		break;
	default:
		break;
	}
}

int FileLoader::LoadGraph(const std::wstring& localPath, AssetType type)
{
	//ロード済みのアセット
	if (assets_.contains(localPath)) {
		return assets_[localPath]->handle;
	}
	//リソースディレクトリのパスと結合して、フルパスを作る
	std::wstring fullPath = resource_dir;
	fullPath += localPath;
	
	//まだロード済みではないアセット
	int h = DxLib::LoadGraph(fullPath.c_str());
	assert(h>=0);
	auto asset = std::make_shared<Asset>();
	asset->fileType = FileType::graphic;
	asset->type = type;//continuosかtemporaryか
	asset->handle = h;//ファイルハンドル
	assets_[localPath] = asset;
	return h;
}

int FileLoader::LoadSound(const std::wstring& localPath, AssetType type)
{
	if (assets_.contains(localPath)) {//もしロード済みなら
		return assets_[localPath]->handle;//そのハンドルを返す
	}
	//リソースディレクトリのパスと結合して、フルパスを作る
	std::wstring fullPath = resource_dir;
	fullPath += localPath;
	
	//まだロード済みではないなら、ロードして登録そしてハンドルを返す
	auto asset=std::make_shared<Asset>();
	auto h = LoadSoundMem(fullPath.c_str());
	assert(h >= 0);
	asset->handle = h;
	asset->type = type;
	asset->fileType = FileType::sound;
	assets_[localPath] = asset;
	return h;
}

void FileLoader::DeleteTemporaryAssets()
{
	std::erase_if(assets_,
					[](const auto& asset) {
						return asset.second->type == AssetType::temporary;//一時オブジェクトだったら削除対象
					});
}
