#pragma once
#include<string>
class StringUtiligy
{
public:
	/// <summary>
	/// string�I�u�W�F�N�g(SJIS)����wstring�I�u�W�F�N�g�ɕϊ�����
	/// </summary>
	/// <param name="str">string�I�u�W�F�N�g</param>
	/// <returns>���C�h�������Ԃ�</returns>
	static std::wstring GetWStringFromString(const std::string& str);
};

