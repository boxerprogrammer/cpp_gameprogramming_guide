#include "ResourceManager.h"
#include<DxLib.h>
#include<cassert>

namespace mylib {
const std::wstring resource_dir = L"../resource/";
}

int mylib::LoadTexture(const std::wstring& path)
{
	//LoadGraph‚ÍŽ¸”s‚·‚é‚Æ-1‚ð•Ô‚·
	int handle = LoadGraph((resource_dir + path).c_str());
    assert(handle != -1);
    return handle;
}

int mylib::LoadSE(const std::wstring& path)
{
	int handle = LoadSoundMem((resource_dir + path).c_str());
    assert(handle != -1);
    return handle;
}
