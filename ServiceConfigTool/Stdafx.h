#pragma once
//ASCII ת Unicode
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
	PAGE_MAIN,				//������
	PAGE_SERVICECONFIG,		//����������
	PAGE_OTHER,		//��������
	PAGE_END,
};