#include "StringUtility.h"
#include<DxLib.h>
#include<cassert>

std::wstring StringUtility::GetWstringFromString(const std::string& str)
{
	//最初の呼び出しでは必要なカウント数だけ取得します。
	auto count = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED |
		MB_ERR_INVALID_CHARS,
		str.c_str(),//WindowsAPIなのでchar*がほしいためc_str()を指定
		static_cast<int>(str.length()),//入力文字列数を指定
		nullptr,//最初の呼び出しでは確定していないためnullptr
		0//ここもサイズが0
	);
	
	//最初の処理がうまくいってれば、countに「必要なワイド文字の数」が入っているはず
	if (count == 0) {
		assert(0);
		return L"";
	}
	std::wstring ret;
	ret.resize(count);//必要なワイド文字数を確保しておく
	MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED |
		MB_ERR_INVALID_CHARS,
		str.c_str(),//元データのアドレス(書き込む側)
		static_cast<int>(str.length()),//元データのサイズ
		ret.data(),//実データのアドレス(書き込まれる側)
		static_cast<int>(ret.length())//書き込まれるサイズ
	);
	return ret;
}
