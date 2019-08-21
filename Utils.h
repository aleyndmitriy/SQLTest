#pragma once

#include<strsafe.h>
#include <string>
#include<vector>
#include <Windows.h>
#include<memory>

std::string Wstr2Str(const std::wstring& wStr);
std::wstring Str2Wstr(const std::string& str);
std::string FindAndReplace(std::string tInput, std::string tFind, const std::string& tReplace);
std::vector<std::string> split(const std::string &text, const std::string& delimeter);
std::string EncryptPassword(const std::string& password);
std::string DecryptPassword(const std::string& password);
