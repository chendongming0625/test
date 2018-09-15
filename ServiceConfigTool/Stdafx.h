#pragma once
//ASCII 转 Unicode
//wstring A2W(const string &AsciiStr);
//
//wstring A2W(const string &AsciiStr)
//{
//	wstring str(AsciiStr.length() * 3, ' ');
//
//	int len = MultiByteToWideChar(CP_ACP, 0, AsciiStr.c_str(), -1, &str[0], str.length());
//
//	str.resize(len - 1);
//	return str;
//}

enum PAGE_TYPE
{
	PAGE_BEGIN,
	PAGE_MAIN,				//主界面
	PAGE_SERVICECONFIG,		//服务器配置
	PAGE_OTHER,		//其他配置
	PAGE_END,
};