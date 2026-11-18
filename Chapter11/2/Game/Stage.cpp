#include "Stage.h"
#include<DxLib.h>
#include"../Application.h"
#include"../FileLoader.h"
#include"Actor.h"

constexpr int chip_width = 32;
constexpr int chip_height = 32;
constexpr float img_scale = 1.0f;
//RotaGraph系はPos=中心座標なので、そのまま表示しようとすると
//半チップぶんずれています。このためズレを計算して補正します
constexpr int adjust_chip_width = (chip_width * img_scale) / 2;
constexpr int adjust_chip_height = (chip_height * img_scale) / 2;

void Stage::TransposeData(const StageData_t& rawData, StageData_t& data)
{
	//それぞれのレイヤーで転置を行う
	for (int layerIdx = 0;layerIdx < rawData.size();++layerIdx) {//レイヤーループ
		//データ部(ここで転置を行う)
		for (int idx = 0;idx < rawData[layerIdx].size();++idx) {
			//現在の１次元データインデックスから、列方向と行方向のインデックスに分解する
			//元データの列インデックス、元データの行インデックスを計算
			int xIdx = idx % mapSize_.w;//元データの横方向番号
			int yIdx = idx / mapSize_.w;//元データの縦方向番号
			//例えば、200x50のマップの場合、256番目のデータは
			//元データでは1行目の56番目になります。xIdx=56,yIdx=1
			//書込み先ではyIdx(1)+xIdx(56)*50=2801番目になります
			data[layerIdx][yIdx + xIdx * mapSize_.h] = //書込み先はy+x*縦幅
				rawData[layerIdx][xIdx + yIdx * mapSize_.w];//元データはx+y*横幅
		}

	}
}

void Stage::Load(int stageIndex)
{
#pragma pack(1)
	struct Header {
		char identifier[4];//4
		uint32_t size;//4
		uint32_t mapWidth;//4
		uint32_t mapHeight;//4
		uint8_t chipWidth;//1
		uint8_t chipHeight;//1
		uint8_t layerCount;//1
		uint8_t bitCount;//1
	};//20バイト(もしかしたらパッキングで24バイトになるかもしれない)
#pragma pack()

	wchar_t chipName[16];//ステージデータファイル名を生成する
	//wsprintfはワイド文字列でフォーマットを行うための関数
	//chipNameに対して("bgchip%d.png",statIndex)のフォーマットで、書き込む
	wsprintf(chipName, L"bgchip%d.png", stageIndex);//チップファイル名が確定
	std::wstring chipPath = L"img/Stage/";
	chipPath += chipName;//直前のフォルダ名をつなげてファイルパスにする
	auto loader = Application::GetInstance().GetLoader();
	//チップ画像のロード
	handle_=loader->LoadGraph(chipPath.c_str(), AssetType::temporary);


	wchar_t stageName[16];
	//これでステージ番号を入れればステージファイル名を知ることができる
	wsprintf(stageName, L"stage%d.fmf", stageIndex);
	std::wstring filePath = L"data/";
	filePath += stageName;//ファイルパス名の生成
	int handle = FileRead_open(filePath.c_str());

	Header header = {};
	FileRead_read(&header, sizeof(header), handle);
	mapSize_.w = header.mapWidth;
	mapSize_.h = header.mapHeight;
	StageData_t rawData;
	//ヘッダデータをもとにステージデータ部をロードします
	//レイヤー数ぶんの枠を確保

	//元データと書込み先データそれぞれレイヤーを確保する(現時点ではレイヤー２個)
	rawData.resize(header.layerCount);//レイヤー数だけ確保
	data_.resize(header.layerCount);//レイヤー数だけ確保
	
	//元データ
	for (auto& d : rawData) {
		//1レイヤー当たりのデータ量を確保
		d.resize(header.mapWidth* header.mapHeight);
		//そこに全データをロードしています
		FileRead_read(d.data(), d.size(), handle);
	}
	//書込み先データ
	for (auto& d : data_) {
		//ここでは必要な分の確保だけ行います。
		d.resize(header.mapWidth * header.mapHeight);
	}
	FileRead_close(handle);//ファイルを閉じる
	//縦横を入れ替える(転置)
	TransposeData(rawData, data_);
}


const Stage::StageData_t& 
Stage::Data() const
{
	return data_;
}

//指定の列のデータを取り出して返す
std::vector<uint8_t> Stage::GetColumn(LayerType layer,int xIdx) const
{
	auto layerIdx = static_cast<int>(layer);
	std::vector<uint8_t> ret;
	ret.resize(mapSize_.h);//一列分だけ確保(返す用)
	auto startIt = data_[layerIdx].begin();//先頭データを指し示す
	//例えば50列目を取ってきたいとすると
	//startItの先頭+50*mapSize_.hの位置に移動します
	startIt += xIdx * mapSize_.h;//指定の列の最初を取ってきます

	//copy_n(元データの先頭,バイト数,書込み先データの先頭);
	//あらかじめ確保しておいた１行分のデータ(ret)に対して
	//元のデータのxIdx列目のデータを(mapSize_.hぶん)コピーする
	std::copy_n(startIt, mapSize_.h, ret.begin());//一列分だけコピー
	return ret;//コピーを返す
}

int Stage::GetCurrentScroll() const
{
	return scroll_;
}

void Stage::Update()
{
	++scroll_;
}

void Stage::Draw()
{
	//地形のデータだけは自分で表示します
	const int stageHeight = mapSize_.h * chip_height * img_scale;
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//ステージの真ん中が画面の真ん中に来るように補正する
	auto adjustHeight = (stageHeight - wsize.h) / 2;

	//１画面に見えるチップの数を計算しておく
	const int viewWidth = wsize.w/chip_width;//1280/32=40が入ってる

	const auto& terrainData = data_[static_cast<int>(LayerType::terrain)];
	//現在のスクロールから、左端のデータインデックスを計算しておく
	int scrollIdx = scroll_ / chip_width;
	//現在のスクロールの左端から、見える範囲分だけ描画する
	//最初は0～40がスクロールして、1～41になるようにする。
	//見えてる範囲だけ描画したい
	for (int xIdx = scrollIdx;xIdx <= scrollIdx + viewWidth;++xIdx) {
		for (int yIdx = 0;yIdx < mapSize_.h;++yIdx) {
			int chipIdx = terrainData[yIdx + xIdx * mapSize_.h];
			if (chipIdx == 0) {
				continue;
			}


			//元データのチップインデックス0列目は無視するが、チップ画像自体は
			//詰め詰めにしているため、X方向0チップのところだけ詰めておく
			//(-1しておく)
			constexpr int chipdata_x_num = 16;
			DrawRectRotaGraph(
				xIdx * chip_width * img_scale + adjust_chip_width - scroll_,
				yIdx * chip_height * img_scale + adjust_chip_height-adjustHeight,
				((chipIdx % chipdata_x_num) - 1) * chip_width, 
				(chipIdx / chipdata_x_num) * chip_height,
				chip_width, chip_height,
				img_scale, 0.0f,
				handle_, true);

		}
	}
}

int Stage::GetChipWidth() const
{
	return chip_width;
}
//地形との当たり判定
bool Stage::IsHitTerrain(std::shared_ptr<Actor> actor, const Vector2& offset) const
{
	//画面サイズの取得
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//チップ全体の高さ
	const int stageHeight = mapSize_.h * chip_height * img_scale;

	//ステージの真ん中が画面の真ん中に来るように補正する
	auto adjustHeight = (stageHeight - wsize.h) / 2;//描画的にずらすY
	auto adjustYStartIdx = adjustHeight/chip_height;//当たる可能性のある部分から開始する

	//１画面に見えるチップの数を計算しておく
	const int viewWidth = wsize.w / chip_width;//1024/32=32が入ってる
	const int viewHeight = wsize.h / chip_height;//578/32=18

	const auto& terrainData = data_[static_cast<int>(LayerType::terrain)];

	int scrollIdx = scroll_ / chip_width;//スクロールに合わせた列インデックスを計算
	//見える範囲の最大インデックスを計算(マップの最大値を超えないように)
	const int xMaxIdx = std::min(scrollIdx + viewWidth, mapSize_.w);
	//縦横の対象ブロックを全部判定します
	for (int xIdx = scrollIdx;xIdx <= scrollIdx + viewWidth;++xIdx) {
		//見える範囲がマップ最大値をオーバーしないようにしている
		const int yMaxIdx = std::min(adjustYStartIdx + viewHeight, mapSize_.h);
		for (int yIdx = adjustYStartIdx;yIdx < yMaxIdx;++yIdx) {
			int chipIdx = terrainData[yIdx + xIdx * mapSize_.h];
			if (chipIdx == 0) {
				continue;
			}
			//チップの中心座標を計算
			Position2 chipPos = { xIdx * chip_width * img_scale + adjust_chip_width * img_scale,
								 yIdx * chip_height * img_scale + adjust_chip_height * img_scale };
			chipPos.x -= scroll_;
			chipPos.y -= adjustHeight;
			//ステージのブロックはアクター扱いにしていないため、アクターなしのコリジョンオブジェクトを
			//一時的に生成して、当たり判定を行う
			auto terrainCol = std::make_shared<Collision>( chipPos,chip_width/2 );
			if (IsHit(actor->GetCollision(), terrainCol,offset)) {
				return true;
			}
		}
	}
	return false;
}

bool Stage::BlockTerrain(std::shared_ptr<Actor> actor, const Vector2& offset) const
{
	//画面サイズの取得
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//チップ全体の高さ
	const int stageHeight = mapSize_.h * chip_height * img_scale;

	//ステージの真ん中が画面の真ん中に来るように補正する
	auto adjustHeight = (stageHeight - wsize.h) / 2;//描画的にずらすY
	auto adjustYStartIdx = adjustHeight / chip_height;//当たる可能性のある部分から開始する

	//１画面に見えるチップの数を計算しておく
	const int viewWidth = wsize.w / chip_width;//1024/32=32が入ってる
	const int viewHeight = wsize.h / chip_height;//578/32=18

	const auto& terrainData = data_[static_cast<int>(LayerType::terrain)];

	int scrollIdx = scroll_ / chip_width;//スクロールに合わせた列インデックスを計算
	//見える範囲の最大インデックスを計算(マップの最大値を超えないように)
	const int xMaxIdx = std::min(scrollIdx + viewWidth, mapSize_.w);
	//縦横の対象ブロックを全部判定します
	for (int xIdx = scrollIdx;xIdx <= scrollIdx + viewWidth;++xIdx) {
		//見える範囲がマップ最大値をオーバーしないようにしている
		const int yMaxIdx = std::min(adjustYStartIdx + viewHeight, mapSize_.h);
		for (int yIdx = adjustYStartIdx;yIdx < yMaxIdx;++yIdx) {
			int chipIdx = terrainData[yIdx + xIdx * mapSize_.h];
			if (chipIdx == 0) {
				continue;
			}
			//チップの中心座標を計算
			Position2 chipPos = { xIdx * chip_width * img_scale + adjust_chip_width * img_scale,
								 yIdx * chip_height * img_scale + adjust_chip_height * img_scale };
			chipPos.x -= scroll_;
			chipPos.y -= adjustHeight;
			//ステージのブロックはアクター扱いにしていないため、アクターなしのコリジョンオブジェクトを
			//一時的に生成して、当たり判定を行う
			auto terrainCol = std::make_shared<Collision>(chipPos, chip_width / 2);
			if (IsHit(actor->GetCollision(), terrainCol, offset)) {
				auto overlapVec=GetOverlap(actor->GetCollision(), terrainCol);
				auto& pos = actor->GetPosition();
				pos += overlapVec*1.5f;
				return true;
			}
		}
	}
	return false;
}
