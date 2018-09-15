#pragma once
#include <Windows.h>
#include <string>
#include <thread>
using namespace std;
class CMsgTip
{
public:
	~CMsgTip();

	void MsgLoop();
	static LRESULT WINAPI MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	SIZE GetTextSize(wstring szText);
	void Paint(HWND hWnd, wstring strMsg);

	static void ShowMsgTip(HWND hParentWnd, HFONT hFont, wstring lpszMsg, UINT nShowTime = 1);
protected:
	CMsgTip(HFONT hFont);

protected:
	int m_nShowTime;
	int m_nCurTime;
	int m_nAlpha;
	HFONT m_hFont;
	SIZE m_cSize;
	wstring m_strMsg;
};