#include "Stage.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>

namespace {
	//+0	identifier[U4]	// ファイル識別子 'FMF_' (0x5F464D46)
	//+ 4	size[U4]	// ヘッダを除いたデータサイズ
	//+ 8	mapWidth[U4]	// マップの横幅
	//+ 12	mapHeight[U4]	// マップの縦幅
	//+ 16	chipHeight[U1]	// パーツの横幅
	//+ 17	chipHeight[U1]	// パーツの縦幅
	//+ 18	layerCount[U1]	// レイヤー数
	//+ 19	bitCount[U1]	// レイヤーデータのビットカウント(8/16)
	//配置に必要な情報
	struct DataSetting {
		uint8_t chipW;//チップ幅(いらない)
		uint8_t chipH;//チップ高さ(いらない)
		uint8_t layerCount;//レイヤー数
		uint8_t bitCount;//１チップに何ビット使用しているのか
	};
	//マップデータのヘッダ
	struct DataHeader {//20バイト
		char identifier[4];//正しければ'FMF_'になっている
		uint32_t size;//データのサイズ
		uint32_t width;//データの幅
		uint32_t height;//データの高さ
		DataSetting setting;//データ配置に必要な情報
	};
}

void Stage::Load(int stageNo)
{
	wchar_t filePath[32];
	wsprintf(filePath, L"data/stage%d.fmf", stageNo);

	auto handle = FileRead_open(filePath);
	assert(handle > 0);
	DataHeader header;
	FileRead_read(&header, sizeof(header), handle);
	
	//利用者側から参照できるように内部変数にコピー
	dataSize_.w = header.width;
	dataSize_.h = header.height;

	//データを受け取る準備をします
	data_.resize(header.size);

	//生データ(データをひっくりかえすために一時的に用意)
	std::vector<uint8_t> rawData;
	rawData.resize(header.size);
	
	//ここでまず、データ本体を一気に読み込みます
	FileRead_read(rawData.data(), rawData.size() * sizeof(uint8_t), handle);
	
	//データが上から下になってるんですが、縦シューは下から上にすすむために
	//データを上下逆にします
	for (int y = 0; y < header.height; ++y) {
		//header.height-yだと範囲がh～1であるためさらに1を引きます
		//書き込み先の先頭をheight-1にしています
		int revY = header.height -1 - y;

		std::copy_n(&rawData[y * header.width],//コピー元の先頭アドレス
					header.width,//コピーする個数
					&data_[revY * header.width]);//コピー先の先頭アドレス
	}
	FileRead_close(handle);
}

Size Stage::MapSize() const
{
	return dataSize_;
}

uint8_t Stage::GetData(int xidx, int yidx)
{
	return data_[yidx*dataSize_.w+xidx];
}

const std::vector<uint8_t>& Stage::GetAllData() const
{
	return data_;
}
