#pragma once

#include<string>

namespace mylib{
class ResourceManager
{
};
int LoadTexture(const std::wstring& path);
int LoadSE(const std::wstring& path);
}//namespace mylib

