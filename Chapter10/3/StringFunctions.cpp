#include "StringFunctions.h"
#include<DxLib.h>

std::wstring StringFunctions::WStringFromString(const std::string& str)
{
	std::wstring ret;//戻り値はワイド文字列

	//MultiByteToWideCharの使い方は、２回呼び出します
	//１回目の呼び出しで、whar_tのサイズを得ます。
	//これをしないと、ワイドの方のメモリをどれくらい確保していいか
	//わからないため

	//最初の呼び出しで、ワイド文字列に必要な文字数を返す
	int size = MultiByteToWideChar(CP_ACP,//もとのマルチバイト文字列のコードページ
		0,//オプションなし
		str.c_str(),//元の文字列のC言語スタイル文字列
		//最後に\0がついている
		str.length(),//文字列の長さ(保険のため)
		nullptr, 0);//今回は受け取り側がないのでnullptr
	//sizeにwchar_tのサイズ(wchar_tがどれくらい必要か)が入っています。
	ret.resize(size);//必要なサイズを確保
	//今度は実際に変換を行います
	size = MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		str.length(),
		ret.data(), //受け取り手の先頭アドレス
		ret.size());//受け取り手のサイズ
	return ret;
}
