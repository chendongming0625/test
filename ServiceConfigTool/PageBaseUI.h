#pragma once
#include "UIlib.h"
#include "resource.h"
#include <thread>
#include "Stdafx.h"
using namespace DuiLib;
class CPageBaseUI :public WindowImplBase
{
public:
	CPageBaseUI();
	~CPageBaseUI();

	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESTYPE GetResourceType()const;
	LPCTSTR GetWindowClassName(void)const;
	virtual void Close(UINT nRet = IDOK);

	void OnFinalMessage(HWND hWnd);
	void Notify(TNotifyUI &msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	CControlUI* GetControl(wstring strControlName);
	virtual void InitWindow();
	virtual void InitUI(int nShowID = 0, LPVOID lpParam = NULL);

private:
	wstring m_strXmlName;
	wstring m_strPageName;
	PAGE_TYPE m_ePageType;
};