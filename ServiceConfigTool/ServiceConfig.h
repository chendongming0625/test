#pragma once
#include "ServiceConfig.h"
#include "SimpleLogger.h"
#include "PageBaseUI.h"
#include "MsgTip.h"
#include "Stdafx.h"
#include <json/json.h>
#include <fstream>
#include <sstream>

#define FILE_NAME "\\..\\data\\etc\\vas-manager.conf"


class CServiceConfigUI :public CPageBaseUI
{
public:
	CServiceConfigUI();
	~CServiceConfigUI();

	static string W2A(const wstring &wstrcode);
	static wstring A2W(const string &AsciiStr);
	void Notify(TNotifyUI &msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void InitUI(int nShowID = 0, LPVOID lpParam = NULL);
	void Close(UINT nRet=1);
	void SaveConfig();
private:
	
	string GetEditContent();
	void SetEditContent();
	CEditUI*m_pService;
	CEditUI*m_pPort;
	CEditUI*m_pSql;

	CEditUI*m_pUserName;
	CEditUI*m_pPassWord;
	string m_strFilePath;

	string m_strService;
	string m_strPort;
	string m_strSql;
	string m_strUserName;
	string m_strPassWord;
};
