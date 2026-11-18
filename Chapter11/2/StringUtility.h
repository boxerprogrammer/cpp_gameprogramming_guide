#pragma once

#include<string>

/// <summary>
/// 文字列関連の便利関数集
/// </summary>
class StringUtility
{
public:
	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換する
	/// </summary>
	/// <param name="str">マルチバイト文字列(非L)</param>
	/// <returns>ワイド文字列(Lつき文字列)</returns>
	static std::wstring GetWstringFromString(const std::string& str);
};

