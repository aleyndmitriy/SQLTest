#pragma once

#include <string>
#include <Windows.h>
#include<memory>

std::string Wstr2Str(const std::wstring& wStr);
std::wstring Str2Wstr(const std::string& str);
std::string FindAndReplace(std::string tInput, std::string tFind, const std::string& tReplace);
